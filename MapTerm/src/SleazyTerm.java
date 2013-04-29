//**************************************************************************************************************
// CLASS: SleazyTerm
//
// VERSION: Fri 23 Nov 2012 1540
//
// DESCRIPTION:
// Implements a very sleazy RS-232 terminal which provides only one function: download a file from the host PC
// to the TWR-MCF5225X microcontroller board.
//
// AUTHOR:
// Kevin R. Burger (burgerk@asu.edu)
// http://kevin.floorsoup.com
//
// REMARKS:
// Uses the RxTx serial I/O library. See http://rxtx.qbang.org.
//
// INSTALLATION:
// The rxtx/32-bit directory contains the RXTX 32-bit Java files downloaded from,
//
//     http://rxtx.qbang.org/wiki/index.php/Download
//
// This is Release rxtx 2.1-7r2 (stable). The rxtx/64-bit directory contains the RXTX 64-bit Java port from
// CloudHopper downloaded from,
//
//     http://www.cloudhopper.com/opensource/rxtx/
//
// This is Version RXTX-2-2-20081207 Windows-x64.
//
// 1. Determine if you are running 32-bit or 64-bit Java by typing: javac -d32 -version
//    If you get an error message saying this Java instance does not support a 32-bit JVM then you are running
//    64-bit Java. Let variable N be 32 or 64 appropriately.
// 2. Create a directory named SleazyTerm.
// 3. Copy SleazyTerm.java to SleazyTerm.
// 4. Copy rxtx/N-bit/RXTXcomm.jar to SleazyTerm.
// 5. Copy rxtx/N-bit/rxtxSerial.dll to SleazyTerm.
// 6. Make SleazyTerm your current directory.
// 7. Make sure the Java JDK bin directory is in your Windows PATH environment variable.
// 8. Build by typing: javac -classpath .\;RXTXcomm.jar SleazyTerm.java
// 9. Run by typing:   java -classpath .\;RXTXcomm.jar SleazyTerm
//
// The new version will automatically try connecting using COM1, COM2, COM3, and COM4 in sequence.
//**************************************************************************************************************
import gnu.io.*;
import java.io.*;

//==============================================================================================================
// CLASS: SleazyTerm
//==============================================================================================================
public class SleazyTerm {
	
	private SerialPort serPort;

    //----------------------------------------------------------------------------------------------------------
    // The packet identifier for a Download Map Packet.
    //----------------------------------------------------------------------------------------------------------
    private static final byte PACKET_ID_DWNLD = (byte)0x05;

    //----------------------------------------------------------------------------------------------------------
    // CTOR: SleazyTerm()
    //
    // DESCRIPTION:
    // Does nothing.
    //----------------------------------------------------------------------------------------------------------
    public SleazyTerm() {
    	serPort = null;
    }

    //----------------------------------------------------------------------------------------------------------
    // METHOD: send()
    //
    // DESCRIPTION:
    // Downloads an 8x8 map to the TWR-MCF5225X microcontroller board.
    //----------------------------------------------------------------------------------------------------------
    public boolean send(byte[][] map) {
    	if(map.length != 8 || map[0].length != 8) {
    		System.out.println("Cannot send a map that isn't 8x8");
    		return false;
    	}
    	
    	int lensent = 0;
    	int ack_count = 0;
    	
        try {
        	/*// Send packet header
        	serPort.getOutputStream().write(0x0C);
        	lensent += 1;
        	
        	while(!did_ack()) {
        		Thread.sleep(100);
        		ack_count++;
        		
        		if(ack_count >= 100)
        			return false;
        	}*/

        	// Send map byte by byte
        	byte mbyte = 0x00;
        	for(int y = 0; y < 8; y++) {
        		for(int x = 0; x < 8; x++) {
        			// Convert map byte to mangled byte
        			/*
        			 * 240 = 0x00 empty
        			 * 255 = 0xFF wall
					 * 244 = 0x60 ghost
					 * 248-252 = 0x85 player
        			 */
        			switch(map[y][x]) {
        			case MapTerm.MAP_EMPTY:
        				mbyte = (byte)0x00;
        				break;
        			case MapTerm.MAP_WALL:
        				mbyte = (byte)0xFF;
        				break;
        			case MapTerm.MAP_GHOST:
        				mbyte = (byte)0x60;
        				break;
        			case MapTerm.MAP_PLAYER:
        				mbyte = (byte)0x85;
        				break;
        			default:
        				mbyte = (byte)0x00;
        				break;
        			}
        			
        			serPort.getOutputStream().write((byte)mbyte);
        			
        			ack_count = 0;
        			while(!did_ack()) {
        				Thread.sleep(100);
        				ack_count++;

        				if(ack_count >= 100) {
        					System.out.println("Incomplete send of " + (lensent) + " bytes. Try again.\n");
        					return false;
        				}
        			}
        			
        			lensent++;
        			
        			System.out.print(map[y][x]);
        			if(x == 7)
        				System.out.print("\n");
        		}
        	}
            System.out.println("Sent " + (lensent) + " bytes.\n");
        } catch (Exception e) {
            System.out.println("Transfer failed. " + e);
            return false;
        }
        
        return true;
    }
    
    public boolean did_ack() {
    	try {
    		//Thread.sleep(10);
    		byte[] ack = read();
    		if(ack != null && ack.length == 1) {
    			System.out.println("Ack recv'd");
    		} else {
    			System.out.println("Ack not recieved");
    			return false;
    		}
    	} catch(Exception e) {
    		System.out.println(e);
    		return false;
    	}
    	
    	return true;
    }
    
    public byte[] read() {
    	byte[] inBuf = new byte[16];
    	byte[] retBuf = null;
    	int len = 0;
    	
    	try {
    		len = serPort.getInputStream().read(inBuf);
    		
    		if(len > 0) {
    			retBuf = new byte[len];
    			for(int i = 0; i < len; i++) {
    				retBuf[i] = inBuf[i];
    				System.out.println("Read: " + Integer.toHexString(retBuf[i]) + ", " + Integer.toBinaryString(retBuf[i]));
    			}
    		}
    		
    		/*if(len > 0) {
    			for(int i = 0; i < len; i++) {
    				System.out.print(Integer.toBinaryString(inBuf[i]) + " = " + inBuf[i] + ". ");
    			}
    			System.out.println();
    		} else {
    			System.out.print("no data");
    		}
    		System.out.println();*/
    	} catch(Exception e) {
    		System.out.println("Read failed " + e);
    	}
    	
    	return retBuf;
    }

    //----------------------------------------------------------------------------------------------------------
    // METHOD: connect()
    //
    // DESCRIPTION:
    // Opens a connection on serial port COM1 at 9600 baud 8N1.
    //----------------------------------------------------------------------------------------------------------
    public void connect() {
    	try {
	        for (int port = 1; port < 5; ++port) {
	            String sComPort = "COM" + port;
	            try {
	                CommPortIdentifier commPortId = CommPortIdentifier.getPortIdentifier(sComPort);
	                System.out.print("Trying to connect on " + sComPort + "...");
	                CommPort cp = commPortId.open(this.getClass().getName(), 2000);
	                System.out.println(" Success");
	                if (cp instanceof SerialPort) {
	                    SerialPort com = (SerialPort)cp;
	                    com.setSerialPortParams(9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
	                    serPort = com;
	                }
	                return;
	            } catch (NoSuchPortException e) {
	                System.out.println(" Failed");
	            }
	        }
    	} catch(Exception e) {
    		System.out.println("Connect() exception: " + e);
    	}
    }
    
    public void disconnect() {
    	try {
    		if(serPort != null) {
    			serPort.close();
    			serPort = null;
    		}
    	} catch(Exception e) {
    		System.out.println("Connect() exception: " + e);
    	}
    }
}
