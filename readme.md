# A fake bank (No UI)
1. first step -- create a local client




what's the flowchart?
client:
    1. net -> connect to sercer; send message to server; receive message from server;
    2. client -> handle all main funciont : Login(), TranscationTrade(), Exit(), ThreadPool(functions)
    3. ui -> show the interface to user(select number to do)
    4. logger -> log the logs
server:
    1. receive the connect
    2. use protocol to deseralize the packet
    3. according to packet's flag to deal with transcation
    4. return the answer(some responed number)

protocol:
    1. handle the packet
    2. seralize the json to str
    3. deseralize the str to json