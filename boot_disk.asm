disk_load:
  pusha

  push  dx

  mov   ah, 0x02  ; read function for 0x13 interrupt
  mov   al, dh    ; number of sectors to read           (0x001 ... 0x080)
  mov   cl, 0x02  ; current sector                      (0x001 ... 0x011)
                  ; 0x01 is boot sector. 0x02 is the first available sector to
                  ; read from
  mov   ch, 0x00  ; cylinder                            (0x000 ... 0x3FF)
  mov   dh, 0x00  ; head number                         (0x000 ... 0x00F)

  int   0x13      ; BIOS interrupt for reading from disk
  jc    disk_err  ; an error from interrupt is stored in carry bit, so if tru
                  ; go to error handling

  pop   dx
  cmp   al, dh    ; compare al to number of sectors read
  jne   sectors_err

  popa
  ret

disk_err:
  mov   bx, DISK_ERR
  call  print
  jmp   disk_loop

sectors_err:
  mov   bx, SECTORS_ERR
  call  print

disk_loop:
  jmp   disk_loop

DISK_ERR:
  db "DISK READ ERROR", 0

SECTORS_ERR:
  db "SECTOR READ ERROR- INCORRECT NUMBER", 0
