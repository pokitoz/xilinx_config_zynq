#!/bin/bash

set -e

# make sure to be in the same directory as this script #########################
current_script_dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )


if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../setup_env.sh
fi



pushd "${current_script_dir}"





cat <<EOF > ./etc_network_interfaces
# /etc/network/interfaces -- configuration file for ifup(8), ifdown(8)

# The loopback interface
auto lo
iface lo inet loopback

iface atml0 inet dhcp

# Wired or wireless interfaces
iface eth0 inet static
address 10.42.0.2
netmask 255.255.255.0
network 10.42.0.0
broadcast 10.42.0.255
gateway 10.42.0.1


EOF



cat <<EOF > ./rc.local
#!/bin/sh -e
#
# rc.local
#
# This script is executed at the end of each multiuser runlevel.
# Make sure that the script will "exit 0" on success or any other
# value on error.
#
# In order to enable or disable this script just change the execution
# bits.
#
# By default this script does nothing.

ifup eth0

echo "Changing root password to root"


sudo date --set "25 Sep 2020 15:00:00"

# set root password
echo -e "root\nroot\n" | passwd root

# allow root SSH login with password
perl -pi -e 's/^(PermitRootLogin) without-password$/$1 yes/g' /etc/ssh/sshd_config


exit 0

EOF
