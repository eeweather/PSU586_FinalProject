# Zack Fravel
# MIPS Instruction Generator for ECE585/586
# Portland State University - Spring 2021
#
# 2021.5.28 - Modified by David Craft to add Mips decode functionality
# provide the op-code type and register decimal number.

# Import tkinter for GUI development
import tkinter as tk
from tkinter import *

# Import textwrap to split binary strings into chunks of 4
import textwrap

# Configure GUI
root = tk.Tk()
root.minsize(450, 150)
root.title('MIPS Instruction Generator for ECE585/586')
Label(root).pack()  # Blank Spacer

# Variables
sel_opcode = StringVar()
sel_rs = StringVar()
sel_rt = StringVar()
sel_rd = StringVar()
sel_immediate = StringVar()
sel_mips = StringVar()

# Binary to Hex Conversion String
HexToBin = {"0": "0000", "1": "0001", "2": "0010", "3": "0011", "4": "0100", "5": "0101", "6": "0110", "7": "0111",
            "8": "1000", "9": "1001", "A": "1010", "B": "1011", "C": "1100", "D": "1101", "E": "1110", "F": "1111"}

BinToHex = {value: key for key, value in HexToBin.items()}


def convertBinaryStringToHex(binaryString):
    split_string = textwrap.wrap(binaryString, 4)
    hex_string = ""
    for chunk in split_string:
        hex_val = BinToHex[chunk]
        hex_string += hex_val
    return hex_string

def convertHexStringToBinary(hexString):
    binary_string = ""
    for chunk in hexString:
        binary_val = HexToBin[chunk]
        binary_string += binary_val
    return binary_string

# Dropdown menu options
opcodes = ['ADD', 'ADDI', 'SUB', 'SUBI', 'MUL', 'MULI', 'OR', 'ORI', 'AND',
           'ANDI', 'XOR', 'XORI', 'LDW', 'STW', 'BZ', 'BEQ', 'JR', 'HALT']

r_type = ['000000','000010','000100','000110','001000','001010']

registers = [str(i) for i in range(32)]

# Default options
sel_opcode.set("ADD")
sel_rs.set("0")
sel_rt.set("0")
sel_rd.set("0")
sel_immediate.set("0")

# Add Dropdown Menu
Label(root, text="Opcode:").pack()
OptionMenu(root, sel_opcode, *opcodes).pack()
Label(root).pack()  # Blank Spacer

# Add Dropdown Menu
Label(root, text="Rs:").pack()
OptionMenu(root, sel_rs, *registers).pack()
Label(root).pack()  # Blank Spacer

# Add Dropdown Menu
Label(root, text="Rt:").pack()
OptionMenu(root, sel_rt, *registers).pack()
Label(root).pack()  # Blank Spacer

# Add Dropdown Menu
Label(root, text="Rd:").pack()
OptionMenu(root, sel_rd, *registers).pack()
Label(root).pack()  # Blank Spacer

# Add Path Field
Label(root, text="Immediate:").pack()
Entry(root, width=20, textvariable=sel_immediate).pack()
Label(root).pack()  # Blank Spacer


def convert_opcode(Op):
    if Op == 'ADD':
        return "000000"
    elif Op == 'ADDI':
        return "000001"
    elif Op == 'SUB':
        return "000010"
    elif Op == 'SUBI':
        return "000011"
    elif Op == 'MUL':
        return "000100"
    elif Op == 'MULI':
        return "000101"
    elif Op == 'OR':
        return "000110"
    elif Op == 'ORI':
        return "000111"
    elif Op == 'AND':
        return "001000"
    elif Op == 'ANDI':
        return "001001"
    elif Op == 'XOR':
        return "001010"
    elif Op == 'XORI':
        return "001011"
    elif Op == 'LDW':
        return "001100"
    elif Op == 'STW':
        return "001101"
    elif Op == 'BZ':
        return "001110"
    elif Op == 'BEQ':
        return "001111"
    elif Op == 'JR':
        return "010000"
    elif Op == 'HALT':
        return "010001"
    else:
        return "000000"

def op_type(Op):
    # r-type
    if Op == '000000':
        return "ADD"
    elif Op == '000010':
        return "SUB"
    elif Op == '000100':
        return "MUL"
    elif Op == '000110':
        return "OR"
    elif Op == '001000':
        return "AND"
    elif Op == '001010':
        return "XOR"
    # i-type:
    elif Op == '000001':
        return "ADDI"
    elif Op == '000011':
        return "SUBI"
    elif Op == '000101':
        return "MULI"
    elif Op == '000111':
        return "ORI"
    elif Op == '001001':
        return "ANDI"
    elif Op == '001011':
        return "XORI"
    elif Op == '001100':
        return "LDW"
    elif Op == '001101':
        return "STW"
    elif Op == '001110':
        return "BZ"
    elif Op == '001111':
        return "BEQ"
    elif Op == '010000':
        return "JR"
    elif Op == '010001':
        return "HALT"
    else:
        return "NOP"

# Function called on each generate graph click
def generate_r_instruction():
    rs = sel_rs.get()
    rt = sel_rt.get()
    rd = sel_rd.get()
    opcode_s = convert_opcode(sel_opcode.get())
    rs_s = "{0:05b}".format(int(rs))
    rt_s = "{0:05b}".format(int(rt))
    rd_s = "{0:05b}".format(int(rd))
    binary_instruction = opcode_s + rs_s + rt_s + rd_s + "00000000000"
    print("\nBinary: ", binary_instruction)
    print("Hex:    ", convertBinaryStringToHex(binary_instruction))
    pass


# Function called on each generate graph click
def generate_i_instruction():
    rs = sel_rs.get()
    rt = sel_rt.get()
    immediate = sel_immediate.get()
    opcode_s = convert_opcode(sel_opcode.get())
    rs_s = "{0:05b}".format(int(rs))
    rt_s = "{0:05b}".format(int(rt))
    imm_s = "{0:016b}".format(int(immediate))
    binary_instruction = opcode_s + rs_s + rt_s + imm_s
    print("\nBinary: ", binary_instruction)
    print("Hex:    ", convertBinaryStringToHex(binary_instruction))
    pass

# Function called on each generate graph click
def decode_Instruction():
    mipsCode = sel_mips.get()
    binaryCode = convertHexStringToBinary(mipsCode)
    print("\nMIPS Decode:    ", binaryCode)
    print(f"Type: {binaryCode[:6]} >> {op_type(binaryCode[:6])}")
    if(binaryCode[:6] in r_type):
        print(f"Rs: {binaryCode[6:11]} >> {int(binaryCode[6:11],2)}")
        print(f"Rt: {binaryCode[11:16]} >> {int(binaryCode[11:16],2)}")
        print(f"Rd: {binaryCode[16:21]} >> {int(binaryCode[16:21],2)}")
    else:
        print(f"Rs: {binaryCode[6:11]} >> {int(binaryCode[6:11],2)}")
        print(f"Rt: {binaryCode[11:16]} >> {int(binaryCode[11:16],2)}")
        print(f"Immediate: {binaryCode[16:32]} >> {int(binaryCode[16:32],2)}")
    pass

# Add Search Button, calls button_clicked()
Button(root, text="Generate R-Type Instruction", command=generate_r_instruction).pack()
Label(root).pack()  # Blank Spacer

# Add Search Button, calls button_clicked()
Button(root, text="Generate I-type Instruction", command=generate_i_instruction).pack()
Label(root).pack()  # Blank Spacer

# Add Mips Code
Label(root, text="Mips Code:").pack()
Entry(root, width=20, textvariable=sel_mips).pack()
Label(root).pack()  # Blank Spacer

# Add Search Button, calls button_clicked()
Button(root, text="Decode Instruction", command=decode_Instruction).pack()
Label(root).pack()  # Blank Spacer

root.mainloop()
