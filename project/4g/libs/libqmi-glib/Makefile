#
# Copyright (C) 2012 Aleksander Morgado <aleksander@lanedo.com>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libqmi-glib
PKG_VERSION:=0.0.18
PKG_RELEASE:=1

PKG_SOURCE:=libqmi-glib-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.lanedo.com/~aleksander/libqmi-glib/

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/libqmi-glib
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+glib2 +zlib
  TITLE:=libqmi-glib
  URL:=http://www.lanedo.com/~aleksander
endef

define Package/libqmi-glib/description
  Helper library and utils to talk to QMI enabled modems
endef

CONFIGURE_VARS+= CFLAGS="$$$$CFLAGS -DMESSAGE_ENABLE_TRACE"

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/include/libqmi-glib \
		$(1)/usr/include/

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/*.{so*,a,la} \
		$(1)/usr/lib/
endef

define Package/libqmi-glib/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/*.so* \
		$(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qmicli $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/qmi-network $(1)/usr/bin/
endef

$(eval $(call BuildPackage,libqmi-glib))
