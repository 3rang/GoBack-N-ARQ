//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

simple Server
{
    parameters:
        @display("i=block/routing");
        int Window_Size;
        
    gates:
        input in;
        output out;
}

simple Client
{
    parameters:
        @display("i=block/process");
    gates:
        input in;
        output out;
}


network GoBackNetwork
{
    submodules:
        Tx: Server {
            parameters:
                @display("i=,cyan;p=49,65");
        }
        Rx: Client {
            parameters:
                @display("i=,gold;p=221,78");
        }
    connections:
        Tx.out --> {  delay = 100ms; datarate=100Mbps; } --> Rx.in;
        Tx.in <-- {  delay = 100ms; datarate=100Mbps; } <-- Rx.out;
}

