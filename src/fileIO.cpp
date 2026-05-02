/**
 *  @name: fileIO.h
 *
 *  @author: Shauna Recto
 *
 *  @description: Header file for input output to the filesystem
 *
 *
 */

#include "fileIO.h"

XDPPortalConfig::XDPPortalConfig() {
	portal = xdp_portal_new();
	returnedFilename = "";
}

XDPPortalConfig::~XDPPortalConfig() {}

bool XDPPortalConfig::openFile() {
	// xdp_portal_open_file(portal, NULL, "Open File", NULL, NULL,
	// XDP_OPEN_FILE_FLAG_NONE, NULL, openFileCallback, NULL);

	return portal != nullptr ? true : false;
}

void XDPPortalConfig::openFileCallback(GObject *source_object,
									   GAsyncResult *res, gpointer data) {
	return;
}