from mininet.net import Mininet
from mininet.node import RemoteController, OVSSwitch
from mininet.cli import CLI
from mininet.log import setLogLevel

def setupTopology():
    net = Mininet(controller=RemoteController, switch=OVSSwitch)

    # Adding SDN Controller
    controller = net.addController('c0', controller=RemoteController, ip="127.0.0.1", port=6653)

    # Adding Routers as Hosts
    r1 = net.addHost('r1', ip='10.0.1.1/24')
    r2 = net.addHost('r2', ip='10.0.2.1/24')
    r3 = net.addHost('r3', ip='10.0.3.1/24')

    # Adding OpenFlow Switch
    s1 = net.addSwitch('s1')

    # Connecting Hosts to Switch
    net.addLink(r1, s1)
    net.addLink(r2, s1)
    net.addLink(r3, s1)

    # Starting the Network
    net.start()
    
    # Testing Reachability
    print("Testing reachability between BGP routers:")
    net.pingAll()

    CLI(net)
    net.stop()

if __name__ == '__main__':
    setLogLevel('info')
    setupTopology()
