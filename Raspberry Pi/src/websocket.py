import os
import sys
import json
import asyncio

import websockets


write_pipe = int(sys.argv[1])
read_pipe = int(sys.argv[2])
read_file = os.fdopen(read_pipe)

async def send_force(uri):
    async with websockets.connect(uri) as websocket:
        while True:
            force = read_file.readline()
            force_json = json.dumps({
                'force': force
            })
            await websocket.send(force_json)

asyncio.run(
    #send_force('ws://192.168.0.3:8000/ws/force_api/')
    send_force('ws://ec2-13-48-129-224.eu-north-1.compute.amazonaws.com/ws/force_api/')
)
