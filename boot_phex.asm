print_hex:
  pusha

  mov   cx, 0             ; index var

; gets the last character in dx register and converts it to ASCII
; for number 0-9:     add 0x30
; for alphabetic A-F: add 0x40
; finally, move the ASCII byte into the correct position on the HEX_OUT str
; (our buffer for final string)
format_hex:
  cmp   cx, 4             ; loop 4 times
  je    print_hex_end

  mov   ax, dx            ; use ax as working register
  and   ax, 0x000f        ; mask first three bytes to zeros (so we just deal
                          ; with one byte at a time)
  add   al, 0x30          ; add 0x30 to convert to ascii character (for 0x0-0x9,
                          ; this is all we need to do)
  cmp   al, 0x39          ; if al register > 0x39, then we know it's A-F case.
  jle   format_hex_2      ;
  add   al, 7             ; because A is at ASCII 65 (instead of 58), add 7 to account
                          ; for offset

; gets the correct position of the string, at whcih we are placing our newly
; formatted ASCII character
; bx register <- base address + string length - index of character
format_hex_2:
  mov   bx, HEX_OUT + 5   ; base value + length
  sub   bx, cx            ; subtract by index variable
  mov   [bx], al          ; copy ASCII in al to position pointed to by bx
  ror   dx, 4             ; rotate right by 1 (0x1234 -> 0x4123)

  add   cx, 1             ; increment index
  jmp   format_hex

print_hex_end:
  mov bx, HEX_OUT         ; move stringified hex value to bx and print
  call print

  popa
  ret

HEX_OUT:
  db "0x0000", 0          ; reserve in memory a space for the string
                          ; representation

