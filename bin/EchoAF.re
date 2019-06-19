open Base;

open Httpaf;
open Httpaf_lwt_unix;

let hello = reqd => {
  Stdio.printf("Listening on port %i and echoing POST requests.\n%!", 8080);
  Reqd.respond_with_string(reqd, Response.create(`OK), "He");
};
let request_handler = (_: Unix.sockaddr) => hello;
let error_handler = (_: Unix.sockaddr, ~request as _=?, error, start_response) => {
  Stdio.printf("Listening on port %i and echoing POST requests.\n%!", 8080);
  ();
};

let on_server_start = _ => {
  Stdio.printf("Listening on port %i and echoing POST requests.\n%!", 8080);
};
let create_server = port => {
  let listen_address = Unix.(ADDR_INET(inet_addr_loopback, port));
  Lwt.async(() =>
    Lwt_io.establish_server_with_client_socket(
      listen_address,
      Server.create_connection_handler(~request_handler, ~error_handler),
    )
    |> Lwt.map(on_server_start)
  );
  let (forever, _) = Lwt.wait();
  Lwt_main.run(forever);
};

let () = {
  create_server(8080);
};