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
import java.util.*;

//==============================================================================================================
// CLASS: SleazyTerm
//==============================================================================================================
public class SleazyTerm {

    //----------------------------------------------------------------------------------------------------------
    // The packet identifier for a Download Song Packet.
    //----------------------------------------------------------------------------------------------------------
    public static final byte PACKET_ID_DWNLD = (byte)0x01;

    //----------------------------------------------------------------------------------------------------------
    // METHOD: main()
    //
    // DESCRIPTION:
    // Execution starts here. Instantiate a SleazyTerm object and call run() on the object.
    //
    // NOTE: We do not catch and handle exceptions. If we did, then this code wouldn't be so sleazy, now would
    // it?
    //----------------------------------------------------------------------------------------------------------
    public static void main(String[] args) throws Exception {
		new SleazyTerm().run();
    }

    //----------------------------------------------------------------------------------------------------------
    // CTOR: SleazyTerm()
    //
    // DESCRIPTION:
    // Does nothing.
    //----------------------------------------------------------------------------------------------------------
    public SleazyTerm() {
        System.out.println("SleazyTerm: Version Fri 23 Nov 2012 1540");
    }

    //----------------------------------------------------------------------------------------------------------
    // METHOD: download()
    //
    // DESCRIPTION:
    // Downloads a song to the TWR-MCF5225X microcontroller board.
    //----------------------------------------------------------------------------------------------------------
    private void download() {
        File file;
        long fileLen;
        String strFilename;
        try {
            strFilename = getFilename();
		    if (strFilename.equals("m") || strFilename.equals("M")) return;
            file = new File(strFilename);
            while (!file.exists()) {
                System.out.println("Cannot open " + strFilename + " for reading. It does not seem to exist.\n");
                strFilename = getFilename();
			    if (strFilename.equals("m") || strFilename.equals("M")) return;
                file = new File(strFilename);
            }
            fileLen = file.length();
        } catch (Exception e) {
            System.out.println("Exception getting file name.");
            return;
        }
        try {
            txPacketHdr(PACKET_ID_DWNLD, fileLen);
            FileInputStream fin = new FileInputStream(file);
            int datum = fin.read();
            while (datum != -1) {
                mSerialOut.write(datum);
                datum = fin.read();
            }
            fin.close();
            System.out.println("Successfully transferred " + (fileLen + 3) + " bytes.\n");
        } catch (Exception e) {
            System.out.println("Transfer failed :(");
        }
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
    // METHOD: getFilename()
    //
    // DESCRIPTION:
    // Prompts for the name of the song file to download.
    //----------------------------------------------------------------------------------------------------------
    private String getFilename() throws Exception {
        Scanner scanner = new Scanner(System.in);
        System.out.print("File to download (enter 'm' to return to menu)? ");
        return scanner.next();
    }

    //----------------------------------------------------------------------------------------------------------
    // METHOD: menu()
    //
    // DESCRIPTION:
    // Displays the main menu.
    //----------------------------------------------------------------------------------------------------------
    private char menu() throws Exception {
        char choice;
        while (true) {
            System.out.println("\nSleazy Term\n----------------------");
            System.out.println("D - Download Song File");
            System.out.println("Q - Quit");
            System.out.print("\n? ");
            Scanner scanner = new Scanner(System.in);
            choice = scanner.next().charAt(0);
            if ("DdQq".indexOf(choice) == -1) {
                System.out.println("No go man, that's not a valid choice.\n");
            } else {
                break;
            }
        }
        return choice;
    }

    //----------------------------------------------------------------------------------------------------------
    // METHOD: run()
    //
    // DESCRIPTION:
    // The main processing loop.
    //----------------------------------------------------------------------------------------------------------
    public void run() throws Exception {
        boolean quit = false;
        connect();
		while (!quit) {
            char operation = menu();
            switch (operation) {
            case 'D':
            case 'd':
                download();
                break;
            case 'Q':
            case 'q':
                quit = true;
                break;
            }
        }
        mSerialOut.close();
        System.out.println("Bye.");
        System.exit(0);
    }

    //----------------------------------------------------------------------------------------------------------
    // METHOD: txPacketHdr()
    //
    // DESCRIPTION:
    // Transmits the packet header.
    //----------------------------------------------------------------------------------------------------------
    private void txPacketHdr(byte pPacketId, long pPacketLen) throws Exception {
        mSerialOut.write(pPacketId);
        mSerialOut.write((byte)(pPacketLen >> 8));
        mSerialOut.write((byte)(pPacketLen));
    }

    private OutputStream mSerialOut;
}
