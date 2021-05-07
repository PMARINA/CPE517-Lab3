"""A useful tool to help understand the encoding of instructions. Will decode instructions and print out the various values in the instruction."""
OPCODE_STRING = "opcode".ljust(10)
FUNC_STRING = "funct".ljust(10)
RS_STRING = "rs".ljust(10)
RT_STRING = "rt".ljust(10)
RD_STRING = "rd".ljust(10)
JUMP_ADDR_STRING = "jump addr".ljust(10)
IMM_STRING = "imm".ljust(10)


def get_input() -> str:
    """Transform hex instruction to binary bit sequence that's 32-bit padded.

    Returns:
        str: the binary string without 0b
    """
    instr = input("What is the instruction?\n>>> ").strip().lower()
    hex_int = int(instr, 16)
    input_binary_string = str(bin(hex_int))[2:].rjust(32, "0")  # Get rid of 0b
    return input_binary_string


def instruction_decode(input_binary_string: str) -> None:
    """Take binary representation of instruction and print the decoding per MIPS format.

    Args:
        input_binary_string (str): The binary representation of the instruction

    Returns:
        [type]: None. Prints the output to console
    """
    prepend_0b = lambda x: "0b" + x.strip()

    def partition(
        start_index: int, end_index: int, in_str: str = input_binary_string
    ) -> str:
        """Partition input binary string and add the 0b at the beginning.

        Args:
            start_index (int): The start index (included)
            end_index (int): The end index (not included)
            in_str (str, optional): The string to partition. Defaults to input_binary_string.

        Returns:
            str: 0b appended partition of binary string passed in
        """
        return prepend_0b(in_str[start_index, end_index])

    opcode = prepend_0b(input_binary_string[0:6])
    if int(opcode, 2) == 0:
        rs_binary_rep = partition(6, 11)
        rt_binary_rep = partition(11, 16)
        rd_binary_rep = partition(16, 21)
        funct = prepend_0b(input_binary_string[-7:])
        print("R-type instruction")
        print(f"{FUNC_STRING}{int(funct, 2)}")
        print(f"{RS_STRING}{int(rs_binary_rep, 2)}")
        print(f"{RT_STRING}{int(rt_binary_rep, 2)}")
        print(f"{RD_STRING}{int(rd_binary_rep, 2)}")
    elif (int(opcode, 2) == 2) or int(opcode, 2) == 3:
        addr = input_binary_string[7:]
        addr = "0b" + addr.strip()
        hex_format_addr = "{0:#0{1}x}".format(int(addr, 2), 10)
        print("J-type instruction")
        print(f"{JUMP_ADDR_STRING}{hex_format_addr}")
    else:
        print("I-type instruction")
        rs_binary_rep = "0b" + input_binary_string[6:11].strip()
        rt_binary_rep = "0b" + input_binary_string[11:16].strip()
        imm = "0b" + input_binary_string[-16:].strip()
        hex_format_imm = "{0:#0{1}x}".format(int(imm, 2), 6)
        print(f"{OPCODE_STRING}{int(opcode, 2)}")
        print(f"{RS_STRING}{int(rs_binary_rep, 2)}")
        print(f"{RT_STRING}{int(rt_binary_rep, 2)}")
        if int(imm, 2) > 32767:
            imm_complement = "0b"
            for i in range(2, len(imm)):
                if imm[i] == "0":
                    imm_complement += "1"
                else:
                    imm_complement += "0"
            print(f"{IMM_STRING}{hex_format_imm}\t{-int(imm_complement, 2)-1}")
        else:
            print(f"{IMM_STRING}{hex_format_imm}\t{int(imm, 2)}")


def main():
    """Take an instruction from cmd and print its decoding."""
    input_binary_string = get_input()
    instruction_decode(input_binary_string)


if __name__ == "__main__":
    main()
