from fsm_gps import FsmGps

""" Main function. Creates and inits a Gps FSM
"""
def main():
    mi_fsm = FsmGps("mi_fsm") # Create FSM
    # mi_fsm.machine.get_graph().draw('fsm_gps.png', prog='dot') 
    mi_fsm.start() # Init state
    while True:
        print("Estado: ", mi_fsm.state)
        mi_fsm.flag_active = int(input("Enter flag_active: "))
        mi_fsm.flag_find_car = int(input("Enter flag_find_car: "))
        mi_fsm.flag_init_gps_record = int(input("Enter flag_init_gps_record: "))
        mi_fsm.fire()

if __name__ == "__main__":
    main()