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

package com.liferay.nativity.modules.contextmenu;

import com.liferay.nativity.control.NativityControl;
import com.liferay.nativity.modules.contextmenu.model.ContextMenuItem;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Dennis Ju
 */
public abstract class ContextMenuControlBase implements ContextMenuControl {

	public ContextMenuControlBase(
		NativityControl nativityControl,
		ContextMenuControlCallback contextMenuControlCallback) {

		this.nativityControl = nativityControl;
		this.contextMenuControlCallback = contextMenuControlCallback;
		
		_contextMenuItems = new ArrayList<ContextMenuItem>();
	}

	@Override
	public void fireAction(long id, String menuText, String[] paths) {
		for (ContextMenuItem contextMenuItem : _contextMenuItems) {
			if (contextMenuItem.getId() == id) {
				contextMenuItem.fireActions(paths);
				
				break;
			}
		}
	}

	@Override
	public String[] getHelpItemsForMenus(String[] paths) {
		return contextMenuControlCallback.getHelpItemsForMenus(paths);
	}

//	@Override
//	public String[] getMenuItems(String[] paths) {
//		return contextMenuControlCallback.getMenuItems(paths);
//	}
	
	@Override
	public List<ContextMenuItem> getMenuItem(String[] paths) {
		List<ContextMenuItem> contextMenuItems = contextMenuControlCallback.getMenuItem(paths);

		_contextMenuItems.clear();

		for (ContextMenuItem contextMenuItem : contextMenuItems) {
			_contextMenuItems.addAll(contextMenuItem.getAllContextMenuItems());			
		}

		return contextMenuItems;
	}

	protected ContextMenuControlCallback contextMenuControlCallback;
	protected NativityControl nativityControl;

	private List<ContextMenuItem> _contextMenuItems;

}