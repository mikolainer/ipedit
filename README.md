# Overview
This library provides a QLineEdit wrapper for comfortable entering an IP v4 or an integer that should be converted to an IP v4.

# Downloads
You can get built demo app from `bin` directory

# Main features
- keep valid count of the separators (0 or 3)
- move the cursor position when you try to remove a separator
- remove all separators when the last digit removed
- insert all separators when the firts separator added
- convert int to IPv4
- clear end of the octet by entering octet separator
- different ways to add separator

# Description
You can just make instance of `IntIpEdit`. This is the preferred and safest way to use this library. But there are another way to do it. If you need to keep using `QLineEdit`, you can use`static void IpIntIpValidator::set_to(QLineEdit* editor = nullptr)` method to set the validator for any `QLineEdit`. Note that the text value should be setted after setting of the validator.

# Info
State: MVP;
Tested env: Qt 6.10.0 MinGW x64 windows kit;
Tested cases: manual input by typing without the text selection;

# Intentions to improve
- add some tests to check manual input by typing with the text selection
- add some tests to check complex changes like inserting from buffer
- make new tests passed
- check other kits to build (compiler, Qt versions, other IDEs)
- make the cursor movable between octets by tab
