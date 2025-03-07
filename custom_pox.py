from pox.core import core
import pox.openflow.libopenflow_01 as of

log = core.getLogger()

# Function to add custom flow rules when a switch connects
def _handle_ConnectionUp(event):
    log.info(f"Switch {event.dpid} connected.")

    # Example Rule 1: Forward all IP packets to port 2
    msg = of.ofp_flow_mod()
    msg.match.dl_type = 0x0800  # Match only IP packets
    msg.actions.append(of.ofp_action_output(port=2))
    event.connection.send(msg)

    # Example Rule 2: Block traffic from a specific MAC address
    drop_msg = of.ofp_flow_mod()
    drop_msg.match.dl_src = "00:00:00:00:00:02"  # Block traffic from h2
    event.connection.send(drop_msg)

    log.info("Custom rules installed on switch.")

def launch():
    core.openflow.addListenerByName("ConnectionUp", _handle_ConnectionUp)
    log.info("Custom POX Controller is running.")
