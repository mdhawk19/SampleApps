package com.nymi.sample.roaming;

import java.sql.BatchUpdateException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import java.sql.ResultSet;

public class NonceModel {
	private Connection connection = null;

	
	public NonceModel() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver").newInstance();
		} catch (IllegalAccessException | InstantiationException | ClassNotFoundException  e) {
			System.err.println("Error initializing Java database driver: " + e.getLocalizedMessage());
			return;
		}
		
		try {
			connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/nymiroaming?autoReconnect=true&useSSL=false","root","Nymi123");
		} catch (SQLException e) {
			System.err.println("Error initializing database connection: " + e.getLocalizedMessage());
		    System.out.println("SQLState: " + e.getSQLState());
		    System.out.println("VendorError: " + e.getErrorCode());
			return;
		}
		
		// Clean up any rows older than 1 minute in the DB to try to protect against DOS attacks
		if (!isConnected()) {
			System.err.println("Error: Database should be connected but is not");
			return;
		}
		Statement statement = null;
		try {
			statement = connection.createStatement();
			statement.executeUpdate("DELETE FROM nonces WHERE created_time < DATE_ADD(CURRENT_TIMESTAMP, INTERVAL -1 MINUTE);");
			System.err.println("Successfully cleared outdated nonces from database");
		} catch (SQLException e) {
			handleSqlException (e);
		}
	}
	
	public boolean isConnected() { return (connection != null);	}

	public String getNonce (String exchange) {
		Statement statement = null;
		try {
			ResultSet rs = null;
			statement = connection.createStatement();
		    rs = statement.executeQuery("SELECT nonce FROM nonces WHERE exchange=\'" + exchange + "\';");
			if (!rs.next())
				return "";
		    String nonce = rs.getString(rs.getRow());
		    System.err.println("Got nonce " + nonce + " for Exchange " + exchange);
			return nonce;
		} catch (SQLException e) {
			handleSqlException (e);
			return "";
		}
	}
	
	public boolean saveNonce (String exchange, String nonce) {
		PreparedStatement prepstatement = null;
		try {
			prepstatement = connection.prepareStatement("INSERT INTO nonces (exchange, nonce) VALUES (?, ?)");
			prepstatement.setString (1, exchange);
			prepstatement.setString (2, nonce);
			prepstatement.addBatch();
			
			int[] updateCounts = prepstatement.executeBatch();
		    if (updateCounts[0] >= 0) {
		        System.err.println("Successfully added nonce for exchange " + exchange);
		        return true;
		      } else if (updateCounts[0] == Statement.SUCCESS_NO_INFO) {
		        System.err.println("Successfully added nonce for exchange " + exchange);
		        return true;
		      } else if (updateCounts[0] == Statement.EXECUTE_FAILED) {
		        System.err.println("Failed to add nonce for exchange " + exchange);
		        return false;
		      }
		} catch (BatchUpdateException e) {
			try {
				connection.rollback();
			} catch (SQLException e1) {
				handleSqlException(e1);
				return false;
			}
		} catch (SQLException e) {
			handleSqlException (e);
			return false;
		} finally {
			try {
				prepstatement.close();
			} catch (SQLException e) {
				handleSqlException(e);
			}
		}
		return false;
	}
	
	public boolean deleteNoncebyExchange (String exchange) {
		PreparedStatement prepstatement = null;
		try {
			prepstatement = connection.prepareStatement("DELETE FROM nonces WHERE exchange = ?;");
			prepstatement.setString (1, exchange);
			prepstatement.addBatch();
			
			int[] updateCounts = prepstatement.executeBatch();
		    if (updateCounts[0] >= 0) {
		        System.err.println("Successfully deleted nonce with exchange " + exchange);
		        return true;
		      } else if (updateCounts[0] == Statement.SUCCESS_NO_INFO) {
			        System.err.println("Successfully deleted nonce with exchange " + exchange);
		        return true;
		      } else if (updateCounts[0] == Statement.EXECUTE_FAILED) {
		        System.err.println("Failed to delete nonce with exchange " + exchange);
		        return false;
		      }
		} catch (BatchUpdateException e) {
			try {
				connection.rollback();
			} catch (SQLException e1) {
				handleSqlException(e1);
				return false;
			}
		} catch (SQLException e) {
			handleSqlException (e);
			return false;
		} finally {
			try {
				prepstatement.close();
			} catch (SQLException e) {
				handleSqlException(e);
			}
		}
		return false;
	}	
	
	void handleSqlException (SQLException e) {
		System.err.println("Database error in " + Thread.currentThread().getStackTrace()[2].getMethodName() + ": " + e.getLocalizedMessage());
	    System.err.println("SQLState: " + e.getSQLState());
	    System.err.println("VendorError: " + e.getErrorCode());
	}
}
