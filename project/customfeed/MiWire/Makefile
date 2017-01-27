##############################################
# OpenWrt Makefile for speaker_client program
#
#
# Most of the variables used here are defined in
# the include directives below. We just need to 
# specify a basic description of the package, 
# where to build our program, where to find 
# the source files, and where to install the 
# compiled program on the router. 
# 
# Be very careful of spacing in this file.
# Indents should be tabs, not spaces, and 
# there should be no trailing whitespace in
# lines that are not commented.
# 
##############################################

include $(TOPDIR)/rules.mk

# Name and release number of this package
PKG_NAME:=MiWire
PKG_RELEASE:=1


# This specifies the directory where we're going to build the program.  
# The root build directory, $(BUILD_DIR), is by default the build_mipsel 
# directory in your OpenWrt SDK directory
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)


include $(INCLUDE_DIR)/package.mk



# Specify package information for this program. 
# The variables defined here should be self explanatory.
define Package/MiWire/default
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Controls the MiWire platform
endef


define Package/MiWire
	$(Package/MiWire/default)
endef

define Package/MiWire/description
	Control software for the MiWire platform
endef


# Specify what needs to be done to prepare for building the package.
# In our case, we need to copy the source files to the build directory.
# This is NOT the default.  The default uses the PKG_SOURCE_URL and the
# PKG_SOURCE which is not defined here to download the source from the web.
# In order to just build a simple program that we have just written, it is
# much easier to do it this way.
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) ./libs/* $(PKG_BUILD_DIR)/
endef


# We do not need to define Build/Configure or Build/Compile directives
# The defaults are appropriate for compiling a simple program such as this one

define Build/Compile
	make -C $(PKG_BUILD_DIR)		\
		$(TARGET_CONFIGURE_OPTS)	\
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)"	\
		LIBS="$(TARGET_LDFLAGS) -L$(BUILD_DIR)/root-ramips/usr/lib" \
		INCLUDE="-I$(BUILD_DIR)/root-ramips/usr/include"
endef


# Specify where and how to install the program. Since we only have one file, 
# the speaker_client executable, install it by copying it to the /bin directory on
# the router. The $(1) variable represents the root directory on the router running 
# OpenWrt. The $(INSTALL_DIR) variable contains a command to prepare the install 
# directory if it does not already exist.  Likewise $(INSTALL_BIN) contains the 
# command to copy the binary file from its current location (in our case the build
# directory) to the install directory.
define Package/MiWire/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/miwire $(1)/bin/
	$(INSTALL_DIR) $(1)/etc/miwire
	$(INSTALL_DIR) $(1)/etc/init.d
	#$(INSTALL_CONF) $(PKG_BUILD_DIR)/miwire.conf $(1)/etc/miwire/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/miwire.init $(1)/etc/init.d/miwire
endef


define Package/MiWire/conffiles
	/etc/miwire/miwire.conf
endef

# This line executes the necessary commands to compile our program.
# The above define directives specify all the information needed, but this
# line calls BuildPackage which in turn actually uses this information to
# build a package.
$(eval $(call BuildPackage,MiWire))