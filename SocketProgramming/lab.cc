// #include <iostream>
// #include <fstream>
// #include <string>

// #include "ns3/core-module.h"
// #include "ns3/network-module.h"
// #include "ns3/internet-module.h"
// #include "ns3/point-to-point-module.h"
// #include "ns3/applications-module.h"
// #include "ns3/error-model.h"
// #include "ns3/tcp-header.h"
// #include "ns3/udp-header.h"
// #include "ns3/enum.h"
// #include "ns3/event-id.h"
// #include "ns3/flow-monitor-helper.h"
// #include "ns3/ipv4-global-routing-helper.h"
// #include "ns3/traffic-control-module.h"
// #include "ns3/netanim-module.h"

// using namespace ns3;

// std::string fileNameRoot = "basic1";    // base name for trace files, etc

// void CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
// {
//   *stream->GetStream () << Simulator::Now ().GetSeconds () << " " <<  newCwnd << std::endl;
// }

// static void
// TraceCwnd ()    // Trace changes to the congestion window
// {
//   AsciiTraceHelper ascii;
//   Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream (fileNameRoot + ".cwnd");
//   Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeBoundCallback (&CwndChange,stream));
// }

// // static Ptr<OutputStreamWrapper> cWndStream;
// // static Ptr<OutputStreamWrapper> ssThreshStream;
// // static uint32_t ssThreshValue;
// // static uint32_t cWndValue;
// // static bool firstCwnd = true;
// // static bool firstSshThr = true;

// // static void
// // CwndTracer(uint32_t oldval, uint32_t newval)
// // {
// //     if (firstCwnd)
// //     {
// //         *cWndStream->GetStream() << "0.0 " << oldval << std::endl;
// //         firstCwnd = false;
// //     }
// //     *cWndStream->GetStream() << Simulator::Now().GetSeconds() << " " << newval << std::endl;
// //     cWndValue = newval;

// //     if (!firstSshThr)
// //     {
// //         *ssThreshStream->GetStream() << Simulator::Now().GetSeconds() << " " << ssThreshValue << std::endl;
// //     }
// // }

// // static void
// // SsThreshTracer(uint32_t oldval, uint32_t newval)
// // {
// //     if (firstSshThr)
// //     {
// //         *ssThreshStream->GetStream() << "0.0 " << oldval << std::endl;
// //         firstSshThr = false;
// //     }
// //     *ssThreshStream->GetStream() << Simulator::Now().GetSeconds() << " " << newval << std::endl;
// //     ssThreshValue = newval;

// //     if (!firstCwnd)
// //     {
// //         *cWndStream->GetStream() << Simulator::Now().GetSeconds() << " " << cWndValue << std::endl;
// //     }
// // }

// // static void TraceCwnd()
// // {
// //     AsciiTraceHelper ascii;
// //     std::string cwnd_tr_file_name = "cwnd-plot.data";
// //     cWndStream = ascii.CreateFileStream(cwnd_tr_file_name.c_str());
// //     Config::ConnectWithoutContext("/NodeList/1/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback(&CwndTracer));
// //     // Simulator::Schedule(Seconds(0.01), &TraceCwnd);
// // }

// // static void TraceSsThresh()
// // {
// //     AsciiTraceHelper ascii;
// //     std::string ssthresh_tr_file_name = "ssthresh.data";
// //     ssThreshStream = ascii.CreateFileStream(ssthresh_tr_file_name.c_str());
// //     Config::ConnectWithoutContext("/NodeList/1/$ns3::TcpL4Protocol/SocketList/0/SlowStartThreshold", MakeCallback(&SsThreshTracer));
// // }

// // static void CwndChange(uint32_t oldCwnd, uint32_t newCwnd)
// // {
// //     std::cout << Simulator::Now().GetSeconds() << "\t" << newCwnd << "\n";
// //     // *cWndStream->GetStream() << Simulator::Now().GetSeconds() << " " << newCwnd << std::endl;
// // }

// int main(int argc, char *argv[])
// {
//     Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TypeId::LookupByName("ns3::TcpNewReno")));
//     NodeContainer nodes;
//     nodes.Create(2);

//     SeedManager::SetSeed (1);
//     SeedManager::SetRun (0);

//     NodeContainer router;
//     router.Create(1);

//     PointToPointHelper s2g;
//     s2g.SetChannelAttribute("Delay", StringValue("2ms"));
//     s2g.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
//     NetDeviceContainer sg;
//     sg = s2g.Install(nodes.Get(0), router.Get(0));

//     PointToPointHelper g2r;
//     g2r.SetChannelAttribute("Delay", StringValue("5ms"));
//     g2r.SetDeviceAttribute("DataRate", StringValue("500Kbps"));
//     NetDeviceContainer gr;
//     gr = g2r.Install(router.Get(0), nodes.Get(1));

//     InternetStackHelper internetStack;
//     internetStack.Install(nodes);
//     internetStack.Install(router);

//     Ipv4AddressHelper address;
//     address.SetBase("10.0.0.0", "255.255.255.0");
//     Ipv4InterfaceContainer isig = address.Assign(sg);
//     address.SetBase("10.1.1.0", "255.255.255.0");
//     Ipv4InterfaceContainer igir = address.Assign(gr);

//     // std::cout<<r.GetAddress(0) << r.GetAddress(1) <<std::endl;
//     // std::cout<<e.GetAddress(0) << e.GetAddress(1);

//     uint16_t port = 9;

//     BulkSendHelper source("ns3::TcpSocketFactory",
//                           InetSocketAddress(igir.GetAddress(1), port));
//     // source.SetAttribute("MaxBytes", UintegerValue(10));

//     ApplicationContainer sourceApps = source.Install(nodes.Get(0));
//     sourceApps.Start(Seconds(0.1));
//     sourceApps.Stop(Seconds(100.0));

//     PacketSinkHelper receiver("ns3::TcpSocketFactory",
//                               InetSocketAddress(Ipv4Address::GetAny(), port));
//     ApplicationContainer sinkApps = receiver.Install(nodes.Get(1));
//     sinkApps.Start(Seconds(0.1));
//     sinkApps.Stop(Seconds(100.0));

//     Ipv4GlobalRoutingHelper::PopulateRoutingTables();

//     AnimationInterface anim("first-animation.xml");
//     anim.SetConstantPosition(nodes.Get(0), 100.0, 0.0);
//     anim.SetConstantPosition(nodes.Get(1), 0.0, 0.0);
//     anim.SetConstantPosition(router.Get(0), 0.0, 50.0);
//     anim.EnablePacketMetadata(true);

//     // std::ofstream ascii;
//     // Ptr<OutputStreamWrapper> ascii_wrap;
//     // ascii.open ((prefix_file_name + "-ascii").c_str ());
//     // ascii_wrap = new OutputStreamWrapper ((prefix_file_name + "-ascii").c_str (),
//     //                                         std::ios::out);
//     // stack.EnableAsciiIpv4All (ascii_wrap);
//     Simulator::Schedule(Seconds(0.00001), &TraceCwnd);
//     // Simulator::Schedule(Seconds(0.00001), &TraceSsThresh);

//     // Ptr<Socket> ns3TcpSocket = Socket::CreateSocket(nodes.Get(0), TcpSocketFactory::GetTypeId()); //source at n0

//     // Trace Congestion window
//     // ns3TcpSocket->TraceConnectWithoutContext("CongestionWindow", MakeCallback(&CwndChange));

//     Simulator::Stop(Seconds(100.0));
//     Simulator::Run();
//     Simulator::Destroy();

//     Ptr<PacketSink> sink = DynamicCast<PacketSink>(sinkApps.Get(0));
//     std::cout << "Total Bytes Received: " << sink->GetTotalRx();
// }

/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * pld ns-3 version of basic1: single sender through a router
 * To run: ./waf --run basic1 
 * To run and set a command-line argument: ./waf --run "basic1 --runtime=10"
 * To enable logging (to stderr), set this in the environment: NS_LOG=TcpReno=level_info
 */

/*
  Network topology:

  A----R----B 

  A--R: 10 Mbps / 10 ms delay
  R--B: 800Kbps / 50 ms delay
  queue at R: size 7
*/

#include <iostream>
#include <fstream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// #define USE_HELPER

using namespace ns3;

std::string fileNameRoot = "basic1"; // base name for trace files, etc

 Ptr<PacketSink> sink;                         /* Pointer to the packet sink application */
 uint64_t lastTotalRx = 0;                     /* The value of the last total received bytes */
 
 void
 CalculateThroughput ()
 {
   Time now = Simulator::Now ();                                         /* Return the simulator's virtual time. */
   double cur = (sink->GetTotalRx () - lastTotalRx) * (double) 8 / 1e5;     /* Convert Application RX Packets to MBits. */
   std::cout << now.GetSeconds () << "s: \t" << cur << " Mbit/s" << std::endl;
   lastTotalRx = sink->GetTotalRx ();
   Simulator::Schedule (MilliSeconds (100), &CalculateThroughput);
 }

void CwndChange(Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
    *stream->GetStream() << Simulator::Now().GetSeconds() << " " << newCwnd << std::endl;
}

static void
TraceCwnd() // Trace changes to the congestion window
{
    AsciiTraceHelper ascii;
    Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream(fileNameRoot + ".cwnd");
    Config::ConnectWithoutContext("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeBoundCallback(&CwndChange, stream));
}

int main(int argc, char *argv[])
{
    int tcpSegmentSize = 1000;

    unsigned int runtime = 20;
    int delayAR = 10;          
    int delayRB = 50;          
    double bottleneckBW = 0.8; 
    double fastBW = 10;        
    uint32_t queuesize = 7;

    CommandLine cmd;
    cmd.AddValue("runtime", "How long the applications should send data", runtime);
    cmd.AddValue("delayRB", "Delay on the R--B link, in ms", delayRB);
    cmd.AddValue("queuesize", "queue size at R", queuesize);
    cmd.AddValue("tcpSegmentSize", "TCP segment size", tcpSegmentSize);

    cmd.Parse(argc, argv);

    std::cout << "queuesize=" << queuesize << ", delayRB=" << delayRB << std::endl;
    NodeContainer all;
    all.Create(3);

    NetDeviceContainer devAR, devRB;
    PointToPointHelper AR, RB;

    AR.SetDeviceAttribute("DataRate", DataRateValue(DataRate(fastBW * 1000 * 1000)));
    AR.SetChannelAttribute("Delay", TimeValue(MilliSeconds(delayAR)));
    devAR = AR.Install(all.Get(0), all.Get(1));

    RB.SetDeviceAttribute("DataRate", DataRateValue(DataRate(bottleneckBW * 1000 * 1000)));
    RB.SetChannelAttribute("Delay", TimeValue(MilliSeconds(delayRB)));
    devRB = RB.Install(all.Get(1), all.Get(2));

    InternetStackHelper internet;
    internet.Install(all.Get(0));
    internet.Install(all.Get(1));
    internet.Install(all.Get(2));

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.0.0.0", "255.255.255.0");

    Ipv4InterfaceContainer ar = ipv4.Assign(devAR);
    ipv4.SetBase("10.0.1.0", "255.255.255.0");
    Ipv4InterfaceContainer rb = ipv4.Assign(devRB);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    std::cout << "A's address: " << ar.GetAddress(0) << std::endl;
    std::cout << "B's address: " << rb.GetAddress(1) << std::endl;
    std::cout << "R's #1 address: " << ar.GetAddress(1) << std::endl;
    std::cout << "R's #2 address: " << rb.GetAddress(0) << std::endl;

    uint16_t Bport = 80;
    Address sinkAaddr(InetSocketAddress(Ipv4Address::GetAny(), Bport));
    PacketSinkHelper sinkA("ns3::TcpSocketFactory", sinkAaddr);
    ApplicationContainer sinkAppA = sinkA.Install(all.Get(2));
    sink = StaticCast<PacketSink> (sinkAppA.Get (0));
    sinkAppA.Start(Seconds(0.01));
    sinkAppA.Stop(Seconds(10.0));

    Address sinkAddr(InetSocketAddress(rb.GetAddress(1), Bport));

    BulkSendHelper sourceAhelper("ns3::TcpSocketFactory", sinkAddr);

    ApplicationContainer sourceAppsA = sourceAhelper.Install(all.Get(0));
    sourceAppsA.Start(Seconds(0.0));
    sourceAppsA.Stop(Seconds(10.0));

    Simulator::Schedule(Seconds(0.00001), &TraceCwnd);
    Simulator::Schedule (Seconds (1.1), &CalculateThroughput);

    Simulator::Stop(Seconds(10.0));
    Simulator::Run();

    Ptr<PacketSink> sink1 = DynamicCast<PacketSink>(sinkAppA.Get(0));
    std::cout << "Total Bytes Received from A: " << sink1->GetTotalRx() << std::endl;
    return 0;
}
