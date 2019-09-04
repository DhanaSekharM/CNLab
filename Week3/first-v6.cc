#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main(int argc, char *argv[])
{
    std::string ver = "v4";
    CommandLine cmd;
    // cmd.AddValue("v", ver);
    cmd.Parse(argc, argv);

    NodeContainer nodes;
    nodes.Create(2);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);

    InternetStackHelper stack;
    InternetStackHelper ipv4Only;
    ipv4Only.SetIpv6StackInstall(false);
    InternetStackHelper ipv6Only;
    ipv6Only.SetIpv4StackInstall(false);

    // if (ver.compare("ipv4"))
    // {
    //     ipv4Only.Install(nodes);
    //     Ipv4AddressHelper address;
    //     address.SetBase("10.1.1.0", "255.255.255.0");
    //     Ipv4InterfaceContainer interfaces = address.Assign(devices);
    // }
    // else

    ipv6Only.Install(nodes);
    Ipv6AddressHelper ipv6;
    ipv6.SetBase(Ipv6Address("2001:db8::"), Ipv6Prefix(64));
    Ipv6InterfaceContainer interfaces = ipv6.Assign(devices);
    // stack.Install(nodes);

    UdpEchoServerHelper echoServer(9);

    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(interfaces.GetAddress(1, 0), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    AnimationInterface anim("ipv6-animation.xml");
    anim.SetConstantPosition(nodes.Get(0), 100.0, 200.0);
    anim.SetConstantPosition(nodes.Get(1), 2.0, 3.0);
    anim.EnablePacketMetadata(true);

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
