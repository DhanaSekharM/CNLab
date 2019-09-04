/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int main(int argc, char *argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    //sets the time resolution to one nanosecond(default)
    Time::SetResolution(Time::NS);

    //enable two logging components that are built into the Echo Client and Echo Server applications:
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //ns-3 Node objects that will represent the computers in the simulation where applications will be installed.
    NodeContainer nodes;
    //create two nodes. The variable nodes store the pointer to the nodes 
    nodes.Create(2);

    //Point to Point topology helper class to set up a point to point link between nodes
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    //Netdevice container to store the node container
    NetDeviceContainer devices;
    // creating, configuring and installing  devices on the nodes
    devices = pointToPoint.Install(nodes);

    //nstall an Internet Stack (TCP, UDP, IP, etc.) on each of the nodes in the node container
    InternetStackHelper stack;
    stack.Install(nodes);

        
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");

    //performs the actual address assignment to the nodes
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    //create a echo server which listens on port no 9    
    UdpEchoServerHelper echoServer(9);

    //install the above echoServer onto a node 1
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0)); //start the server at time t=1s
    serverApps.Stop(Seconds(10.0)); //stop the server at time t=10s

    //create a udp echo client with the assigned ip addresss
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    Simulator::Run();  //start the simulation
    Simulator::Destroy (); //deallocate all the resources allocated during the simulation
    return 0;
}
