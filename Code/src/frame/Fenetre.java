package frame;

import javax.swing.JFrame;

public class Fenetre {
	
	public void window() {
		JFrame window = new JFrame ("JAVA");
		int x = 500;
		int y = 600;
		window.setSize(x,y);
		window.setVisible(true);
	}
}
