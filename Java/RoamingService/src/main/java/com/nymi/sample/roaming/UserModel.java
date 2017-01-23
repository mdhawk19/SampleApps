package com.nymi.sample.roaming;

import java.sql.BatchUpdateException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import java.sql.ResultSet;

public class UserModel {
	private Connection connection = null;

	
	public UserModel() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver").newInstance();
		} catch (IllegalAccessException | InstantiationException | ClassNotFoundException  e) {
			System.err.println("Error initializing Java database driver: " + e.getLocalizedMessage());
			return;
		}
		
		try {
			connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/nymiroaming?autoReconnect=true&useSSL=false&autocommit=false","root","Nymi123");
		} catch (SQLException e) {
			System.err.println("Error initializing database connection: " + e.getLocalizedMessage());
		    System.out.println("SQLState: " + e.getSQLState());
		    System.out.println("VendorError: " + e.getErrorCode());
			return;
		}
	}
	
	public boolean isConnected() { return (connection != null);	}

	public String getUserKeyFromKeyID (String keyid) {
		Statement statement = null;
		try {
			ResultSet rs = null;
			statement = connection.createStatement();
		    rs = statement.executeQuery("SELECT bandpublickey FROM users WHERE keyid=\'" + keyid + "\';");
			if (!rs.next())
				return "";
		    String pubkey = rs.getString(rs.getRow());
		    System.err.println("Got public key " + pubkey + " for KeyID " + keyid);
			return pubkey;
		} catch (SQLException e) {
			handleSqlException (e);
			return "";
		}
	}
	
	public String getUserIDFromKeyID (String keyid) {
		Statement statement = null;
		try {
			ResultSet rs = null;
			statement = connection.createStatement();
		    rs = statement.executeQuery("SELECT userid FROM users WHERE keyid=\'" + keyid + "\';");
			if (!rs.next())
				return "";
		    String user = rs.getString(rs.getRow());
		    System.err.println("Got user ID " + user + " for KeyID " + keyid);
			return user;
		} catch (SQLException e) {
			handleSqlException (e);
			return "";
		}
	}

	public boolean createNewUser (String userid, String publickey, String keyid) {
		PreparedStatement prepstatement = null;
		
		System.err.println("Adding public key (size " + publickey.length() + "): " + publickey);
		
		try {
			prepstatement = connection.prepareStatement("INSERT INTO users (keyid, userid, bandpublickey) VALUES (?, ?, ?)");
			prepstatement.setString (1, keyid);
			prepstatement.setString (2, userid);
			prepstatement.setString (3, publickey);
			prepstatement.addBatch();
			
			int[] updateCounts = prepstatement.executeBatch();
		    if (updateCounts[0] >= 0) {
		        System.err.println("Successfully added user " + userid);
		        return true;
		      } else if (updateCounts[0] == Statement.SUCCESS_NO_INFO) {
		        System.err.println("Successfully added user " + userid);
		        return true;
		      } else if (updateCounts[0] == Statement.EXECUTE_FAILED) {
		        System.err.println("Failed to add user " + userid);
		        return false;
		      }
		} catch (BatchUpdateException e) {
//			try {
//				connection.rollback();
//			} catch (SQLException e1) {
//				handleSqlException(e1);
//				return false;
//			}
			handleSqlException(e);
			return false;
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
	
	public boolean deleteUserByKeyID (String keyid) {
		PreparedStatement prepstatement = null;
		try {
			prepstatement = connection.prepareStatement("DELETE FROM users WHERE keyid = ?;");
			prepstatement.setString (1, keyid);
			prepstatement.addBatch();
			
			int[] updateCounts = prepstatement.executeBatch();
		    if (updateCounts[0] >= 0) {
		        System.err.println("Successfully deleted user with keyid " + keyid);
		        return true;
		      } else if (updateCounts[0] == Statement.SUCCESS_NO_INFO) {
		        System.err.println("Successfully deleted user with keyid " + keyid);
		        return true;
		      } else if (updateCounts[0] == Statement.EXECUTE_FAILED) {
		        System.err.println("Failed to delete user with keyid " + keyid);
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

	public boolean deleteUserByUserID (String userid) {
		PreparedStatement prepstatement = null;
		try {
			prepstatement = connection.prepareStatement("DELETE FROM users WHERE userid = ?;");
			prepstatement.setString (1, userid);
			prepstatement.addBatch();
			
			int[] updateCounts = prepstatement.executeBatch();
		    if (updateCounts[0] >= 0) {
		        System.err.println("Successfully deleted user " + userid);
		        return true;
		      } else if (updateCounts[0] == Statement.SUCCESS_NO_INFO) {
		        System.err.println("Successfully deleted user  " + userid);
		        return true;
		      } else if (updateCounts[0] == Statement.EXECUTE_FAILED) {
		        System.err.println("Failed to delete user " + userid);
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
