/*
 * Stellarium
 * Copyright (C) 2006 Fabien Chereau
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
 
#ifndef _STELMODULEMGR_HPP_
#define _STELMODULEMGR_HPP_

#include <QObject>
#include <QMap>
#include <QList>
#include "StelModule.hpp"

//! Get a module from its QObject name
#define GETSTELMODULE( m ) StelApp::getInstance().getModuleMgr().getModule( m )

//! @class StelModuleMgr
//! Manage a collection of StelModules including both core and plugin modules.
//! 
class StelModuleMgr : public QObject
{
	Q_OBJECT
			
public:
	StelModuleMgr();
	~StelModuleMgr();
	
	//! Regenerate calling lists if necessary
	void update();
	
	//! Register a new StelModule to the list
	//! The module is later referenced by its QObject name.
	void registerModule(StelModule* m, bool generateCallingLists=false);
	
	//! Unregister and delete a StelModule. The program will hang if other modules depend on the removed one
	//! @param moduleID the unique ID of the module, by convention equal to the class name
	void unloadModule(const QString& moduleID);
	
	//! Load dynamically a module
	//! @param moduleID the name of the module = name of the dynamic library file without extension 
	//! (e.g "mymodule" for mymodule.so or mymodule.dll)
	//! @return the loaded module or NULL in case of error. The returned Stelmodule still needs to be initialized 
	StelModule* loadPlugin(const QString& moduleID);

	//! Unload all plugins
	void unloadAllPlugins();
	
	//! Get the corresponding module or NULL if can't find it.
	//! @param moduleID the QObject name of the module instance, by convention it is equal to the class name
	StelModule* getModule(const QString& moduleName);
	
	//! Get the list of all the currently registered modules
	QList<StelModule*> getAllModules() {return modules.values();}
	
	//! Generate properly sorted calling lists for each action (e,g, draw, update)
	//! according to modules orders dependencies
	void generateCallingLists();

	//! Get the list of modules in the correct order for calling the given action
	const QList<StelModule*>& getCallOrders(StelModule::StelModuleActionName action)
	{
		return callOrders[action];
	}

	//! Contains the information read from the module.ini file
	struct PluginDescriptor
	{
		//! The name of the directory and of the lib*.so with *=key
		QString key;
		QString name;
		QString author;
		QString contact;
		QString description;
		//! If true, the module is automatically loaded at startup
		bool loadAtStartup;
	};
 
	//! Return the list of all the external module found in the modules directories
	static QList<PluginDescriptor> getPluginsList();

	//! Enum used when selecting objects to define whether to add to, replace, or remove from the existing selection list.
	enum SelectAction
	{
		AddToSelection,
		ReplaceSelection,
		RemoveFromSelection
	};

 
private:
	//! The main module list associating name:pointer
	QMap<QString, StelModule*> modules;
	
	//! The list of all module in the correct order for each action
	QMap<StelModule::StelModuleActionName, QList<StelModule*> > callOrders;

	//! True if modules were removed, and therefore the calling list need to be regenerated
	bool callingListsToRegenerate;
};

#endif // _STELMODULEMGR_HPP_
