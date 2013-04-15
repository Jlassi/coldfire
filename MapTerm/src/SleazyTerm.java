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
	
	private OutputStream mSerialOut;

    //----------------------------------------------------------------------------------------------------------
    // The packet identifier for a Download Map Packet.
    //----------------------------------------------------------------------------------------------------------
    private static final byte PACKET_ID_DWNLD = (byte)0xFF;

    //----------------------------------------------------------------------------------------------------------
    // CTOR: SleazyTerm()
    //
    // DESCRIPTION:
    // Does nothing.
    //----------------------------------------------------------------------------------------------------------
    public SleazyTerm() {
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
    	
        try {
        	connect();
        	
        	// Send packet header
        	txPacketHdr(PACKET_ID_DWNLD);
        	
        	// Send the map row by row
        	for(int x = 0; x < 8; x++) {
        		mSerialOut.write(map[x]);
        	}
            
            System.out.println("Successfully transferred " + (64 + 1) + " bytes.\n");
            mSerialOut.close();
        } catch (Exception e) {
            System.out.println("Transfer failed :(");
            return false;
        }
        
        return true;
    }

    //----------------------------------------------------------------------------------------------------------
    // METHOD: connect()
    //
    // DESCRIPTION:
    // Opens a connection on serial port COM1 at 9600 baud 8N1.
    //----------------------------------------------------------------------------------------------------------
    private void connect() throws Exception {
        for (int port = 1; port < 5; ++port) {
            String sComPort = "COM" + port;
            System.out.print("Trying to connect on " + sComPort + "...");
            try {
                CommPortIdentifier commPortId = CommPortIdentifier.getPortIdentifier(sComPort);
                CommPort commPort = commPortId.open(this.getClass().getName(), 2000);
                System.out.println(" Success");
                if (commPort instanceof SerialPort) {
                    SerialPort com = (SerialPort)commPort;
                    com.setSerialPortParams(9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
                    mSerialOut = com.getOutputStream();
                }
                return;
            } catch (NoSuchPortException e) {
                System.out.println(" Failed");
            }
        }
    }

    //----------------------------------------------------------------------------------------------------------
    // METHOD: txPacketHdr()
    //
    // DESCRIPTION:
    // Transmits the packet header.
    //----------------------------------------------------------------------------------------------------------
    private void txPacketHdr(byte pPacketId) throws Exception {
        mSerialOut.write(pPacketId);
    }
}
