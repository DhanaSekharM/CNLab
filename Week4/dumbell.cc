#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/point-to-point-dumbbell.h"
#include "ns3/netanim-module.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/pfifo-fast-queue-disc.h"

using namespace ns3;

std::stringstream filePlotQueue;
std::stringstream filePlotQueueAvg;
uint32_t checkTimes;
double avgQueueSize;
void CheckQueueSize(Ptr<QueueDisc> queue)
{
    uint32_t qSize = queue->GetCurrentSize().GetValue();

    avgQueueSize += qSize;
    checkTimes++;

    // check queue size every 1/100 of a second
    Simulator::Schedule(Seconds(1), &CheckQueueSize, queue);

    std::ofstream fPlotQueue(filePlotQueue.str().c_str(), std::ios::out | std::ios::app);
    fPlotQueue << Simulator::Now().GetSeconds() << " " << qSize << std::endl;
    fPlotQueue.close();

    std::ofstream fPlotQueueAvg(filePlotQueueAvg.str().c_str(), std::ios::out | std::ios::app);
    fPlotQueueAvg << Simulator::Now().GetSeconds() << " " << avgQueueSize / checkTimes << std::endl;
    fPlotQueueAvg.close();
}

int main(int argc, char *argv[])
{
    // Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpNewReno"));
    std::string transport_protocol = "TcpNewReno";
    CommandLine cmd;
    cmd.AddValue("transport_protocol", "Transport protocol to use", transport_protocol);
    transport_protocol = std::string("ns3::") + transport_protocol;
    Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(TypeId::LookupByName(transport_protocol)));

    uint32_t nLeft = 1, nRight = 1;
    PointToPointHelper p2pRouter;
    p2pRouter.SetChannelAttribute("Delay", StringValue("1ms"));
    p2pRouter.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    PointToPointHelper p2pLeaf;
    p2pLeaf.SetChannelAttribute("Delay", StringValue("1s"));
    p2pLeaf.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    PointToPointDumbbellHelper p2pDumbell(nLeft, p2pLeaf,
                                          nRight, p2pLeaf,
                                          p2pRouter);

    InternetStackHelper stack;
    p2pDumbell.InstallStack(stack);

    Ipv4AddressHelper ipv4Left;
    ipv4Left.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4AddressHelper ipv4Right;
    ipv4Right.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4AddressHelper ipv4Router;
    ipv4Router.SetBase("10.1.3.0", "255.255.255.0");
    p2pDumbell.AssignIpv4Addresses(ipv4Left, ipv4Right, ipv4Router);

    OnOffHelper clientHelper("ns3::TcpSocketFactory", Address());
    clientHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    ApplicationContainer rightApps;

    for (uint32_t i = 0; i < Min(nLeft, nRight); i++)
    {
        AddressValue remoteAddress(InetSocketAddress(p2pDumbell.GetLeftIpv4Address(i), 1000));
        clientHelper.SetAttribute("Remote", remoteAddress);
        rightApps.Add(clientHelper.Install(p2pDumbell.GetRight(i)));
    }

    rightApps.Start(Seconds(1.0));
    rightApps.Stop(Seconds(10.0));

    p2pDumbell.BoundingBox(1, 1, 100, 100);

    TrafficControlHelper tch;
    std::cout<<"here";
    tch.Uninstall(p2pDumbell.GetLeft()->GetDevice(0));
    tch.Uninstall(p2pDumbell.GetLeft()->GetDevice(1));
    tch.SetRootQueueDisc("ns3::PfifoFastQueueDisc", "MaxSize", StringValue ("1000p"));
    tch.SetQueueLimits("ns3::DynamicQueueLimits", "HoldTime", StringValue("4ms"));
    QueueDiscContainer queueLeft = tch.Install(p2pDumbell.GetLeft()->GetDevice(0));
    QueueDiscContainer queueRight = tch.Install(p2pDumbell.GetLeft()->GetDevice(1));

    filePlotQueue << "."
                  << "/"
                  << "red-queue.plotme";
    filePlotQueueAvg << "."
                     << "/"
                     << "red-queue_avg.plotme";

    remove(filePlotQueue.str().c_str());
    remove(filePlotQueueAvg.str().c_str());
    Ptr<QueueDisc> queue = queueLeft.Get(0);
    Simulator::ScheduleNow(&CheckQueueSize, queue);

    AnimationInterface anim("dumbell-animation.xml");
    anim.EnablePacketMetadata(true);
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Simulator::Stop(Seconds(10.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}