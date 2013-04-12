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

package com.liferay.nativity.modules.contextmenu.model;

import com.fasterxml.jackson.annotation.JsonIgnore;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Dennis Ju
 */
public class ContextMenuItem {

	public void addAction(Action action) {
		_actions.add(action);
	}
	
	private List<Action> _actions;
	
	public void fireActions(String[] paths) {
		for (Action action : _actions) {
			action.onSelection(paths);
		}
	}

	private String _helpText;

	public String getHelpText() {
		return _helpText;
	}

	public void setHelpText(String helpText) {
		_helpText = helpText;
	}
	
	public ContextMenuItem(String title) {
		_title = title;
		_enabled = true;
		_id = _getUniqueID();

		_actions = new ArrayList<Action>();
		_contextMenuItems = new ArrayList<ContextMenuItem>();
	}
	
	public ContextMenuItem(String title, ContextMenuItem parentContextMenuItem) {
		this(title);

		parentContextMenuItem.addContextMenuItem(this);
	}
		
	private String _title;

	public boolean getEnabled() {
		return _enabled;
	}
	
	public void setEnabled(boolean enabled) {
		_enabled = enabled;
	}
	
	public List<ContextMenuItem> getContextMenuItems() {
		return _contextMenuItems;
	}
	
	public String getTitle() {
		return _title;
	}
	
	public boolean addContextMenuItem(ContextMenuItem menuItem) {
		return _contextMenuItems.add(menuItem);
	}

	public void addContextMenuItem(ContextMenuItem menuItem, int index) {
		_contextMenuItems.add(index, menuItem);
	}

	public boolean removeContextMenuItem(ContextMenuItem menuItem) {
		return _contextMenuItems.remove(menuItem);
	}

	public boolean addSeparator() {
		return _contextMenuItems.add(_getSeparator());
	}

	public void addSeparator(int index) {
		_contextMenuItems.add(index, _getSeparator());
	}
	
	public void setTitle(String title) {
		_title = title;
	}
	
	private List<ContextMenuItem> _contextMenuItems; 
	
	private static final String _SEPARATOR = "_SEPARATOR_";
	
	private boolean _enabled;

	
	private static ContextMenuItem _getSeparator() {
		return new ContextMenuItem(_SEPARATOR);
	}
	
	public long getId() {
		return _id;
	}

	private long _id;
	
	private static synchronized long _getUniqueID() {
		return _globalId++;
    }
	
	@JsonIgnore
	public List<ContextMenuItem> getAllContextMenuItems() {
		List<ContextMenuItem> contextMenuItems = new ArrayList<ContextMenuItem>();

		contextMenuItems.add(this);
		
		addChildren(this, contextMenuItems);
		
		return contextMenuItems;
	}
	
	private void addChildren(ContextMenuItem contextMenuItem, List<ContextMenuItem> contextMenuItems) {
		for (ContextMenuItem childContextMenuItem : contextMenuItem.getContextMenuItems()) {
			contextMenuItems.add(childContextMenuItem);

			addChildren(childContextMenuItem, contextMenuItems);
		}
	}

	private static long _globalId = 0;
	
}
