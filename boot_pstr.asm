print:
  pusha           ; push all general purpose registers to the stack so we can
                  ; "scope" them to these functions

start:
  mov   al, [bx]  ; move bytes from bx pointer into al
  cmp   al, 0     ; compare al to 0, jump to done if ZF flag true
  je    done

  mov   ah, 0x0e  ; set ah to tty mode
  int 0x10        ; 10h interrupt

  add bx, 1       ; increment pointer
  jmp start       ; loop

done:
  popa            ; restore all general-purpose registers from the stack once
                  ; done
  ret

print_nl:
  pusha

  mov   ah, 0x0e
  mov   al, 0x0a  ; print newline character
  int   0x10
  mov   al, 0x0d  ; print carriage return
  int   0x10

  popa
  ret
