NOTE - To run the attacks our test bed setup is required.

Record and Replay attack:
1. Connect the RF Transmitter to one arduino.
2. Connect the RF Receiver and MCP2515 to another arduino.
3. Connect an LED with 1000ohms resistor and an MCP2515 to another arduino UNO.
4. Upload the codes for the three nodes.
5. The initial setup works.
6. Now for the attacker node:
	i.   Connect an MCP2515 to another arduino and connect it to the CAN bus.
	ii.  Upload the code with the Sniffer snippet uncommented.
	iii. Go through the console log and figure out the id and payload of the node whose
	     data you wish to record and replay
	iv.  Once this is done, comment out the sniffer snippet, uncomment the transmitter snippet
	     , modify the can_id and canMsg.data values as per the recorded values and upload the code.
	v.   Attack successful.



Spoof attack:
1. Setup the testbed by connecting CAN transmitter and CAN receiver
2. Attack Ultrasonic range sensor to the transmitter.
3. Upload respective code to each node.
4. Check working of the Ultrasonic sensor by bringing an object to its proximity, verify the beep signal and read the values.
5. Attack:
	i.   Introduce the attacker to network.
	ii.  Sniff the messages being transferred and analyse the message format
	iii. Create data frames for attack in the above read format and transfer it to receiver node.
	iv.  Verify output through beep signal and console monitor.



Mitigation: 
1. Set up the testbed.
2. Connect the MCP2515 to the Arduino
3. Connect the MCP2515 which is connected to the Arduino to the test bed.
4. This will be a new node called as the monitoring node.
5. Upload the security monitoring and mutual authentication code to the monitoring node.
6. Upload the mitigation implemented  transmitter and receiver code to the nodes that are connected to the testbed.
7. Open the console log in the monitoring node, it will display 
  i. Warnings to unidentified node
  ii. Authentication successful for genuine nodes.