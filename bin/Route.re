open Opium.Std;

let hello =
  get("/hello/:name", req => {
    let name = param(req, "name");
    `String("Hello " ++ name) |> respond';
  });
let tuturu =
  post("/", req => {
    let%lwt body = App.string_of_body_exn(req);
    switch (body) {
    | "Mayushii" => `String("Tuturu") |> respond'
    | _ => Rock.Handler.not_found(req)
    };
  });
let () = {
  App.empty |> hello |> tuturu |> App.run_command;
};