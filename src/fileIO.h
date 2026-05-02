/*
 *  @name: fileIO.h
 *
 *  @author: Shauna Recto
 *
 *  @description: Header file for input output to the filesystem
 *
 */

#ifndef FILE_IO_H
#define FILE_IO_H

#include <libportal/portal.h>

#include <string>

class XDPPortalConfig {
  private:
	XdpPortal *portal;
	GVariant *options;

	std::string returnedFilename;

  public:
	XDPPortalConfig();
	~XDPPortalConfig();

	bool openFile();
	static void openFileCallback(GObject *, GAsyncResult *, gpointer);
};

#endif