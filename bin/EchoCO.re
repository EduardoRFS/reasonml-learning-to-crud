open Lwt;
open Cohttp;
open Cohttp_lwt_unix;

Console.log("Running Test Program:");

let server = {
  let callback = (__conn, req, body) => {
    let%lwt req_body = body |> Cohttp_lwt.Body.to_string;
    let body = String.equal(req_body, "") ? "TUTURU" : req_body;
    Server.respond_string(~status=`OK, ~body, ());
  };
  Server.create(~mode=`TCP(`Port(8000)), Server.make(~callback, ()));
};

let () = ignore(server |> Lwt_main.run);