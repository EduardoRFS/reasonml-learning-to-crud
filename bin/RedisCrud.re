open Lwt;
open Opium.Std;

let host = "localhost";
let port = 6379;

let conn_promise = Redis_lwt.Client.connect({host, port});

type data = {
  key: string,
  conn: Redis_lwt.Client.connection,
};
let create_method = (method, action) =>
  method("/:key", req => {
    let key = param(req, "key");
    let%lwt conn = conn_promise;
    let%lwt data = action({key, conn}, req);
    switch (data) {
    | Some(value) => `String(value) |> respond'
    | None => Rock.Handler.not_found(req)
    };
  });
let get = create_method(get);
let put = create_method(put);
let delete = create_method(delete);
let get_key =
  get(({key, conn}, req) =>
    Lwt.map(
      fun
      | Some(value) => Some(key ++ ": " ++ value)
      | None => None,
      Redis_lwt.Client.get(conn, key),
    )
  );
let set_key =
  put(({key, conn}, req) => {
    let%lwt value = App.string_of_body_exn(req);
    let%lwt success = Redis_lwt.Client.set(conn, key, value);
    Lwt.return(success ? Some("SET " ++ key ++ " " ++ value) : None);
  });
let delete_key =
  delete(({key, conn}, req) => {
    let%lwt value = App.string_of_body_exn(req);
    let%lwt deleteds = Redis_lwt.Client.del(conn, [key]);
    Lwt.return(deleteds != 0 ? Some("DEL " ++ key) : None);
  });
let () = {
  App.empty |> get_key |> set_key |> delete_key |> App.run_command;
};