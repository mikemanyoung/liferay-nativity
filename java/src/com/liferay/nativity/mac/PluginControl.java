/**
 * Copyright (c) 2000-2012 Liferay, Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */

package com.liferay.nativity.mac;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.Map;
import java.util.Map.Entry;

public class PluginControl {

	/**
	 * Initialize connection with native service
	 *
	 * @return true if connection is successful
	 */
	public boolean connect() {
		try {
			_serviceSocket = new Socket("127.0.0.1", 33001);

			_serviceBufferedReader = new BufferedReader(
				new InputStreamReader(_serviceSocket.getInputStream()));

			_serviceOutputStream = new DataOutputStream(
				_serviceSocket.getOutputStream());

			_callbackSocket = new Socket("127.0.0.1", 33002);

			_callbackBufferedReader = new BufferedReader(
				new InputStreamReader(_callbackSocket.getInputStream()));

			_callbackOutputStream = new DataOutputStream(
				_callbackSocket.getOutputStream());

			_callbackThread = new ReadThread(this);
			_callbackThread.start();
		}
		catch (IOException e) {
			return false;
		}

		return true;
	}

	/**
	 * disconnects from plugin service
	 */
	public void disconnect() {
		try {
			_serviceSocket.close();
		}
		catch (IOException e) {
		}
	}

	/**
	 * Enable/Disable icon overlay feature
	 *
	 * @param enable pass true is overlay feature should be enabled
	 */
	public void enableOverlays(boolean enable) {
		String command = "enableOverlays:" + (enable ? "1" : "0");

		_sendCommand(command);
	}

	/**
	 * Register icon in the service
	 *
	 * @param path to icon file
	 *
	 * @return registered icon id or 0 in case error
	 */
	public int registerIcon(String path) {
		String command = "registerIcon:" + path;

		String reply = _sendCommand(command);

		return Integer.parseInt(reply);
	}

	/**
	 * Remove icon overlay from file (previously set by setIconForFile)
	 *
	 * @param name of file
	 */
	public void removeFileIcon(String fileName) {
		String command = "removeFileIcon:" + fileName;

		_sendCommand(command);
	}

	/**
	 * Remove icon overlays from files (previously set by setIconForFile)
	 *
	 * @param array of files
	 */
	public void removeFileIcon(String[] fileNames) {
		StringBuilder sb = new StringBuilder();
		
		sb.append("removeFileIcons");

		for (String fileName : fileNames) {
			sb.append(":");
			sb.append(fileName);
		}

		_sendCommand(sb.toString());	
	}

	/**
	 * Set title of root context menu item, all other items will be added as
	 * children of it
	 *
	 * @param new title of item
	 */
	public void setContextMenuTitle(String title) {
		String command = "setMenuTitle:" + title;

		_sendCommand(command);
	}

	/**
	 * Associate icon with fileName
	 *
	 * @param target file name
	 * @param id of icon that should be associated with file
	 */
	public void setIconForFile(String fileName, int iconId) {
		String command = "setFileIcon:" + fileName + ":" + iconId;

		_sendCommand(command);
	}

	/**
	 * Associate icons with multiple fileNames.
	 * 
	 * @param map containing icon id values keyed by file name
	 */
    public void setIconsForFiles(
    	Map<String, Integer> fileIconsMap) {

    	StringBuilder sb = new StringBuilder();

    	sb.append("setFileIcons");

    	int i = 0;

    	for (Entry<String, Integer> entry : fileIconsMap.entrySet()) {
    		sb.append(":");
    		sb.append(entry.getKey());
    		sb.append(":");
    		sb.append(entry.getValue());

    		i++;

    		if (i == _messageBufferSize) {
    			_sendCommand(sb.toString());

    			sb = new StringBuilder();

    	    	sb.append("setFileIcons"); 

    			i = 0;
    		}
    	}

    	if (i > 0) {
    		_sendCommand(sb.toString());	    		
    	}
    }

	/**
	 * Unregister icon in the service
	 *
	 * @param id of icon previously registered by registerIcon method
	 */
	public void unregisterIcon(int id) {
		String command = "unregisterIcon:" + id;

		_sendCommand(command);
	}

	/**
	 * Callback method called by native plugin when context menu executed on one
	 * or more files. User code can override this method to add a number of
	 * additional items to context menu.
	 *
	 * @param array of file names on which context menu executed
	 *
	 * @return array of menu items that should be added to context menu, or null
	 *         if additional context menu not needed
	 */
	protected String[] getMenuItems(String[] files) {
		return null;
	}

	/**
	 * Callback method that executes when user selects custom menu item
	 *
	 * @param index of menu item (index in the array returned by previous
	 *        getMenuItems call)
	 * @param files array on which context menu item executed
	 */
	protected void menuItemExecuted(int index, String[] files) {
	}

	private void _doCallbackLoop() {
		while (_callbackSocket.isConnected()) {
			try {
				String data = _callbackBufferedReader.readLine();

				if (data.startsWith("menuQuery:")) {
					String currentFiles = data.substring(10, data.length());

					_currentFiles = currentFiles.split(":");

					String[] items = getMenuItems(_currentFiles);

					String itemsStr = new String();

					if (items != null) {
						for (int i=0; i<items.length; ++i) {
							if (i > 0)
								itemsStr += ":";

							itemsStr += items[i];
						}
					}

					_callbackOutputStream.writeBytes(itemsStr + "\r\n");
				}

				if (data.startsWith("menuExec:")) {
					menuItemExecuted(
						Integer.parseInt(data.substring(9, data.length())),
						_currentFiles);
				}
			}
			catch (IOException e) {
			}
		}
	}

	private String _sendCommand(String command) {
		try {
			command += "\r\n";
			
			_serviceOutputStream.writeBytes(command);
			
			String reply = _serviceBufferedReader.readLine();

			return reply;
		}
		catch (IOException e) {
			return null;
		}	
	}

	private static long _messageBufferSize = 500;

	private BufferedReader _callbackBufferedReader;

	private DataOutputStream _callbackOutputStream;

	private Socket _callbackSocket;

	private ReadThread _callbackThread;

	private String[] _currentFiles;

	private BufferedReader _serviceBufferedReader;

	private Socket _serviceSocket;

	private DataOutputStream _serviceOutputStream;

	private class ReadThread extends Thread {

		public ReadThread(PluginControl pluginControl) {
			_pluginControl = pluginControl;
		}

		@Override
		public void run() {
			_pluginControl._doCallbackLoop();
		}

		private PluginControl _pluginControl;

	}

}