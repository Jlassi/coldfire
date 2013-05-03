import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import java.awt.GridLayout;

import javax.swing.JFileChooser;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.JLabel;
import java.awt.Font;
import java.io.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;


public class MapTerm extends JFrame {
	// Static tile types understood by the pacman program on the board
	public static final byte MAP_EMPTY = 0x00;
	public static final byte MAP_WALL = 0x01;
	public static final byte MAP_PLAYER = 0x02;
	public static final byte MAP_GHOST = 0x03;

	// Default map
	private static final byte[][] DEFAULT_MAP = new byte[][]{
		{ MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL },
		{ MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
		{ MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_PLAYER, MAP_WALL },
		{ MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
		{ MAP_WALL, MAP_EMPTY, MAP_WALL, MAP_WALL, MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
		{ MAP_WALL, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
		{ MAP_WALL, MAP_GHOST, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_EMPTY, MAP_WALL },
		{ MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL, MAP_WALL }
	};

	private SleazyTerm term;

	private JPanel contentPane;
	private JLabel lblStatus;
	private JTextField[][] tfs;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					MapTerm frame = new MapTerm();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public MapTerm() {
		setTitle("CSE325 Pacman Map Editor");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 514, 325);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);

		JPanel mapInputPane = new JPanel();
		mapInputPane.setBounds(5, 22, 488, 188);
		contentPane.add(mapInputPane);
		mapInputPane.setLayout(new GridLayout(8, 8, 5, 5));

		JPanel controlPanel = new JPanel();
		controlPanel.setBounds(5, 221, 488, 61);
		contentPane.add(controlPanel);
		controlPanel.setLayout(null);

		JButton btnLoad = new JButton("Load");
		btnLoad.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				final JFileChooser fc = new JFileChooser();
				fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
				if(fc.showOpenDialog(contentPane) == JFileChooser.APPROVE_OPTION) {
					loadFile(fc.getSelectedFile().getAbsolutePath());
				}
			}
		});
		btnLoad.setBounds(10, 11, 89, 23);
		controlPanel.add(btnLoad);

		JButton btnSave = new JButton("Save");
		btnSave.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				final JFileChooser fc = new JFileChooser();
				fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
				if(fc.showSaveDialog(contentPane) == JFileChooser.APPROVE_OPTION) {
					saveFile(fc.getSelectedFile().getAbsolutePath());
				}
			}
		});
		btnSave.setBounds(109, 11, 89, 23);
		controlPanel.add(btnSave);

		/*JButton btnReadRs = new JButton("read rs232");
		btnReadRs.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				term.connect();
				term.read();
				term.disconnect();
			}
		});
		btnReadRs.setBounds(209, 11, 89, 23);
		controlPanel.add(btnReadRs);*/

		lblStatus = new JLabel("Ready");
		lblStatus.setFont(new Font("Tahoma", Font.PLAIN, 11));
		lblStatus.setBounds(10, 40, 468, 14);
		controlPanel.add(lblStatus);

		JButton btnSend = new JButton("Send (RS232)");
		btnSend.addActionListener(new ActionListener() {
			/*
			 * Converts the text field grid to a 2d byte array and sends it over the RS232 terminal
			 */
			public void actionPerformed(ActionEvent arg0) {
				if(!checkGrid())
					return;

				lblStatus.setText("Sending. Please wait...");
				
				// Execute the send on a different thread
				EventQueue.invokeLater(new Runnable() { 
					@Override
					public void run() {
						byte[][] map = new byte[8][8];
						for(int y = 0; y < 8; y++) {
							for(int x = 0; x < 8; x++) {
								map[y][x] = textToTile(x, y);
							}
						}

						term.connect();

						if(term.send(map))
							lblStatus.setText("Send succeeded. Press PB1 on the board to play!");
						else
							lblStatus.setText("Send failed. Ensure the board is connected and in PAUSE mode (PB1)");

						term.disconnect();
					}
				});
			}
		});
		btnSend.setBounds(356, 11, 122, 23);
		controlPanel.add(btnSend);

		JLabel lblKey = new JLabel("W = Wall, P = Pacman, G = Ghost, (Blank) = Free Space");
		lblKey.setBounds(5, 7, 334, 14);
		contentPane.add(lblKey);

		term = new SleazyTerm();

		tfs = new JTextField[8][8];
		for(int y = 0; y < 8; y++) {
			for(int x = 0; x < 8; x++) {
				tfs[y][x] = new JTextField();
				mapInputPane.add(tfs[y][x]);
				tfs[y][x].setColumns(1);
			}
		}

		loadDefaultMap();
	}

	/*
	 * Loop through the text field grid and ensure that there are only valid characters on the map
	 * and that there is only one ghost and one pacman spawn point
	 * Also update lblStatus if there are any issues
	 * 
	 * @return true if no problems were detected, false if otherwise
	 */
	private boolean checkGrid() {
		String str;
		int ghostCount = 0, playerCount = 0;
		for(int y = 0; y < 8; y++) {
			for(int x = 0; x < 8; x++) {
				str = tfs[y][x].getText().toUpperCase();

				if(str.equals("W")) {
				} else if(str.equals("G")) {
					ghostCount++;
				} else if(str.equals("P")) {
					playerCount++;
				} else if(str.isEmpty()) {
				} else {
					lblStatus.setText("Error: Unknown tile type \"" + str + "\" at x = " + x + ", y = " + y);
					return false;
				}
			}
		}

		if(playerCount != 1 || ghostCount != 1) {
			lblStatus.setText("Error: There must be exactly 1 ghost and 1 player spawn point");
			return false;
		}

		return true;
	}

	/*
	 * Converts a text field on the grid to a byte representation understood by the board's pacman program
	 */
	private byte textToTile(int x, int y) {
		String str = tfs[y][x].getText();
		byte ret = MAP_EMPTY;
		str = str.toUpperCase();

		if(str.equals("W")) {
			ret = MAP_WALL;
		} else if(str.equals("G")) {
			ret = MAP_GHOST;
		} else if(str.equals("P")) {
			ret = MAP_PLAYER;
		} else {
			ret = MAP_EMPTY;
		}

		return ret;
	}

	/*
	 * Converts a tile byte to a text field string 
	 */
	private String tileToText(byte tile) {
		if(tile == MAP_WALL) {
			return "W";
		} else if(tile == MAP_GHOST) {
			return "G";
		} else if(tile == MAP_PLAYER) {
			return "P";
		} else {
			return "";
		}
	}

	/*
	 * Reads a file containing 64 bytes of map data and displays it into the text field grid
	 */
	private void loadFile(String name) {
		try {
			InputStream in = new BufferedInputStream(new FileInputStream(name));
			byte tile = MAP_EMPTY;
			for(int y = 0; y < 8; y++) {
				for(int x = 0; x < 8; x++) {
					tile = (byte)in.read();
					tfs[y][x].setText(tileToText(tile));
				}
			}
			in.close();
		} catch(Exception ex) {
			lblStatus.setText("Could not read file " + name);
			System.out.println(ex);
			return;
		}

		lblStatus.setText("Loaded file " + name + " successfully");
	}

	/*
	 * Converts the text field grid and saves it as a 64 byte file representing the map
	 */
	private void saveFile(String name) {
		if(!checkGrid())
			return;

		try {
			OutputStream out = new BufferedOutputStream(new FileOutputStream(name));
			for(int y = 0; y < 8; y++) {
				for(int x = 0; x < 8; x++) {
					out.write(textToTile(x, y));
				}
			}
			out.close();
		} catch(Exception ex) {
			lblStatus.setText("Could not write file " + name);
			System.out.println(ex);
			return;
		}

		lblStatus.setText("Saved file " + name + " successfully");
	}

	/*
	 * Sets the default map in the text field grid
	 */
	private void loadDefaultMap() {
		for(int y = 0; y < 8; y++) {
			for(int x = 0; x < 8; x++) {
				tfs[y][x].setText(tileToText(DEFAULT_MAP[y][x]));
			}
		}

		lblStatus.setText("Loaded default map");
		//contentPane.repaint();
	}
}
