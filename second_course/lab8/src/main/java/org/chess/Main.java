package org.chess;

import org.chess.gui.controller.GuiController;
import org.chess.gui.view.GuiView;
import org.chess.model.Board;
import org.chess.player.Player;

import javax.swing.*;

public class Main {
    public static void main(String[] args) {

        SwingUtilities.invokeLater(() -> {

            String playerName = JOptionPane.showInputDialog(
                    null,
                    "Enter your name:",
                    "Chess Game",
                    JOptionPane.QUESTION_MESSAGE
            );

            if (playerName == null || playerName.trim().isEmpty()) {
                playerName = "Player";
            }

            Player player = new Player();
            player.setName(playerName);
            Board board = new Board();
            GuiController controller = new GuiController(board, player);
            GuiView view = new GuiView(controller, board);

            controller.setView(view);

            view.setVisible(true);
        });
    }
}