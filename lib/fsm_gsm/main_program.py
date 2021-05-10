
from fsm_gsm import FsmGsm

""" Main function. Creates and inits a Gps FSM
"""
def main():
	mi_fsm = FsmGsm("mi_fsm") # Create FSM
	mi_fsm.machine.get_graph().draw('fsm_gsm.png', prog='dot') 
	mi_fsm.start() # Init state
	while True:
		print("Estado: ", mi_fsm.state)
		mi_fsm.flag_data_available = int(input("Enter flag_data_available: "))
		mi_fsm.gsm_data = input("Enter gsm_data: ")
		mi_fsm.fire()

if __name__ == "__main__":
	main()