# Sample script fragment shows how to grab text input
#
# Author: Bob Jacobsen, copyright 2009
# Part of the JMRI distribution
#
# The next line is maintained by CVS, please don't change it
# $Revision: 27263 $

import java
import javax.swing

# get a string from the user
inputValue = javax.swing.JOptionPane.showInputDialog("Please input a value");

# show it as string
print inputValue

# convert to a number
inputNumber = int(inputValue)

# show you can do arithmetic with this
print 3+inputNumber-3

