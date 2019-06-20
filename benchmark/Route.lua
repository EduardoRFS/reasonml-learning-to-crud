wrk.method = "POST"
wrk.body   = "Mayushii"
wrk.headers["Content-Type"] = "text/plain"

--[[
wrk -c 64 -t 64 -s ./benchmark/Route.lua -d 10s http://localhost:3000/
--]]