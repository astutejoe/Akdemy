sseg SEGMENT STACK
	byte 4000h DUP(?)
sseg ENDS

dseg SEGMENT PUBLIC
	byte 4000h DUP(?)
	sword ?
	sword ?
	sword ?
	sword ?
	sword ?
	byte 100h DUP(?)
dseg ENDS

cseg SEGMENT PUBLIC
	ASSUME CS:cseg, DS:dseg

strt:
	mov ax, +1
	mov DS:[16384], ax
	mov ax, +1
	mov DS:[16386], ax
	mov ax, +2
	mov DS:[16392], ax
	mov bx, 256
	mov al, 69
	mov DS:[bx], al
	add bx, 1
	mov al, 115
	mov DS:[bx], al
	add bx, 1
	mov al, 99
	mov DS:[bx], al
	add bx, 1
	mov al, 114
	mov DS:[bx], al
	add bx, 1
	mov al, 101
	mov DS:[bx], al
	add bx, 1
	mov al, 118
	mov DS:[bx], al
	add bx, 1
	mov al, 97
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 111
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 110
	mov DS:[bx], al
	add bx, 1
	mov al, 117
	mov DS:[bx], al
	add bx, 1
	mov al, 109
	mov DS:[bx], al
	add bx, 1
	mov al, 101
	mov DS:[bx], al
	add bx, 1
	mov al, 114
	mov DS:[bx], al
	add bx, 1
	mov al, 111
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 100
	mov DS:[bx], al
	add bx, 1
	mov al, 101
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 116
	mov DS:[bx], al
	add bx, 1
	mov al, 101
	mov DS:[bx], al
	add bx, 1
	mov al, 114
	mov DS:[bx], al
	add bx, 1
	mov al, 109
	mov DS:[bx], al
	add bx, 1
	mov al, 111
	mov DS:[bx], al
	add bx, 1
	mov al, 115
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 40
	mov DS:[bx], al
	add bx, 1
	mov al, 109
	mov DS:[bx], al
	add bx, 1
	mov al, 97
	mov DS:[bx], al
	add bx, 1
	mov al, 105
	mov DS:[bx], al
	add bx, 1
	mov al, 111
	mov DS:[bx], al
	add bx, 1
	mov al, 114
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 113
	mov DS:[bx], al
	add bx, 1
	mov al, 117
	mov DS:[bx], al
	add bx, 1
	mov al, 101
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 50
	mov DS:[bx], al
	add bx, 1
	mov al, 41
	mov DS:[bx], al
	add bx, 1
	mov al, 58
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 24h
	mov DS:[bx], al
	mov dx, 256
	mov ah, 09h
	int 21h
	mov ah, 02h
	mov dl, 0Dh
	int 21h
	mov dl, 0Ah
	int 21h
	mov dx, 256
	mov al, 0FFh
	mov DS:[256], al
	mov ah, 0Ah
	int 21h
	mov ah, 02h
	mov dl, 0Dh
	int 21h
	mov dl, 0Ah
	int 21h
	mov di, 258
	mov ax, 0
	mov cx, 10
	mov dx, 1
	mov bl, DS:[di]
	cmp bl, 2Dh
	jne R3
	mov dx, -1
	add di, 1
	mov bl, DS:[di]
R3:
	push dx
	mov dx, 0
R1:
	mov bl, DS:[di]
	cmp bl, 0Dh
	je R2
	imul cx
	add bl, -48
	mov bh, 0
	add ax, bx
	add di, 1
	jmp R1
R2:
	pop cx
	imul cx
	mov DS:[16390], al
	mov DS:[16391], ah
	mov al, DS:[16390]
	mov ah, DS:[16391]
	mov DS:[515], al
	mov DS:[516], ah
	mov al, 2
	mov DS:[517], al
	mov al, DS:[515]
	mov ah, DS:[516]
	mov bl, DS:[517]
	mov bh, 0
	cmp ax, bx
	jg R6
	mov al, 0
	jmp R7
R6:
	mov al, 255
R7:
	mov DS:[517], al
	mov al, DS:[517]
	mov ah, 0
	cmp ax, 0
	je R4
	mov bx, 518
	mov al, 91
	mov DS:[bx], al
	add bx, 1
	mov al, 24h
	mov DS:[bx], al
	mov dx, 518
	mov ah, 09h
	int 21h
	mov al, DS:[16384]
	mov ah, DS:[16385]
	mov DS:[774], al
	mov DS:[775], ah
	mov di, 776
	mov al, DS:[774]
	mov ah, DS:[775]
	mov cx, 0
	cmp ax, 0
	jge R8
	mov bl, 2Dh
	mov DS:[di], bl
	add di, 1
	neg ax
R8:
	mov bx, 10
R9:
	add cx, 1
	mov dx, 0
	idiv bx
	push dx
	cmp ax, 0
	jne R9
R10:
	pop dx
	add dx, 30h
	mov DS:[di], dl
	add di, 1
	add cx, -1
	cmp cx, 0
	jne R10
	mov dl, 24h
	mov DS:[di], dl
	mov dx, 776
	mov ah, 09h
	int 21h
	mov bx, 1032
	mov al, 44
	mov DS:[bx], al
	add bx, 1
	mov al, 24h
	mov DS:[bx], al
	mov dx, 1032
	mov ah, 09h
	int 21h
	mov al, DS:[16386]
	mov ah, DS:[16387]
	mov DS:[1288], al
	mov DS:[1289], ah
	mov di, 1290
	mov al, DS:[1288]
	mov ah, DS:[1289]
	mov cx, 0
	cmp ax, 0
	jge R11
	mov bl, 2Dh
	mov DS:[di], bl
	add di, 1
	neg ax
R11:
	mov bx, 10
R12:
	add cx, 1
	mov dx, 0
	idiv bx
	push dx
	cmp ax, 0
	jne R12
R13:
	pop dx
	add dx, 30h
	mov DS:[di], dl
	add di, 1
	add cx, -1
	cmp cx, 0
	jne R13
	mov dl, 24h
	mov DS:[di], dl
	mov dx, 1290
	mov ah, 09h
	int 21h
	mov al, DS:[16390]
	mov ah, DS:[16391]
	mov DS:[518], al
	mov DS:[519], ah
	mov al, 2
	mov DS:[520], al
	mov al, DS:[518]
	mov ah, DS:[519]
	mov bl, DS:[520]
	mov bh, 0
	cmp ax, bx
	jg R16
	mov al, 0
	jmp R17
R16:
	mov al, 255
R17:
	mov DS:[520], al
	mov al, DS:[520]
	mov ah, 0
	cmp ax, 0
	je R14
	mov bx, 521
	mov al, 44
	mov DS:[bx], al
	add bx, 1
	mov al, 24h
	mov DS:[bx], al
	mov dx, 521
	mov ah, 09h
	int 21h
	jmp R15
R14:
R15:
R18:
	mov al, DS:[16392]
	mov ah, DS:[16393]
	mov DS:[521], al
	mov DS:[522], ah
	mov al, DS:[16390]
	mov ah, DS:[16391]
	mov DS:[523], al
	mov DS:[524], ah
	mov al, DS:[521]
	mov ah, DS:[522]
	mov bl, DS:[523]
	mov bh, DS:[524]
	cmp ax, bx
	jl R20
	mov al, 0
	jmp R21
R20:
	mov al, 255
R21:
	mov DS:[523], al
	mov al, DS:[523]
	mov ah, 0
	cmp ax, 0
	je R19
	mov al, DS:[16386]
	mov ah, DS:[16387]
	mov DS:[525], al
	mov DS:[526], ah
	mov al, DS:[525]
	mov ah, DS:[526]
	mov DS:[16388], al
	mov DS:[16389], ah
	mov al, DS:[16384]
	mov ah, DS:[16385]
	mov DS:[525], al
	mov DS:[526], ah
	mov al, DS:[16386]
	mov ah, DS:[16387]
	mov DS:[527], al
	mov DS:[528], ah
	mov al, DS:[525]
	mov ah, DS:[526]
	mov bl, DS:[527]
	mov bh, DS:[528]
	add ax, bx
	mov DS:[527], al
	mov DS:[528], ah
	mov al, DS:[527]
	mov ah, DS:[528]
	mov DS:[16386], al
	mov DS:[16387], ah
	mov al, DS:[16386]
	mov ah, DS:[16387]
	mov DS:[525], al
	mov DS:[526], ah
	mov di, 527
	mov al, DS:[525]
	mov ah, DS:[526]
	mov cx, 0
	cmp ax, 0
	jge R22
	mov bl, 2Dh
	mov DS:[di], bl
	add di, 1
	neg ax
R22:
	mov bx, 10
R23:
	add cx, 1
	mov dx, 0
	idiv bx
	push dx
	cmp ax, 0
	jne R23
R24:
	pop dx
	add dx, 30h
	mov DS:[di], dl
	add di, 1
	add cx, -1
	cmp cx, 0
	jne R24
	mov dl, 24h
	mov DS:[di], dl
	mov dx, 527
	mov ah, 09h
	int 21h
	mov al, DS:[16392]
	mov ah, DS:[16393]
	mov DS:[525], al
	mov DS:[526], ah
	mov al, DS:[16390]
	mov ah, DS:[16391]
	mov DS:[527], al
	mov DS:[528], ah
	mov al, 1
	mov DS:[529], al
	mov al, DS:[527]
	mov ah, DS:[528]
	mov bl, DS:[529]
	mov bh, 0
	sub ax, bx
	mov DS:[529], al
	mov DS:[530], ah
	mov al, DS:[525]
	mov ah, DS:[526]
	mov bl, DS:[529]
	mov bh, DS:[530]
	cmp ax, bx
	jl R27
	mov al, 0
	jmp R28
R27:
	mov al, 255
R28:
	mov DS:[529], al
	mov al, DS:[529]
	mov ah, 0
	cmp ax, 0
	je R25
	mov bx, 530
	mov al, 44
	mov DS:[bx], al
	add bx, 1
	mov al, 24h
	mov DS:[bx], al
	mov dx, 530
	mov ah, 09h
	int 21h
	jmp R26
R25:
R26:
	mov al, DS:[16392]
	mov ah, DS:[16393]
	mov DS:[530], al
	mov DS:[531], ah
	mov al, 1
	mov DS:[532], al
	mov al, DS:[530]
	mov ah, DS:[531]
	mov bl, DS:[532]
	mov bh, 0
	add ax, bx
	mov DS:[532], al
	mov DS:[533], ah
	mov al, DS:[532]
	mov ah, DS:[533]
	mov DS:[16392], al
	mov DS:[16393], ah
	mov al, DS:[16388]
	mov ah, DS:[16389]
	mov DS:[530], al
	mov DS:[531], ah
	mov al, DS:[530]
	mov ah, DS:[531]
	mov DS:[16384], al
	mov DS:[16385], ah
	jmp R18
R19:
	mov bx, 521
	mov al, 93
	mov DS:[bx], al
	add bx, 1
	mov al, 24h
	mov DS:[bx], al
	mov dx, 521
	mov ah, 09h
	int 21h
	mov ah, 02h
	mov dl, 0Dh
	int 21h
	mov dl, 0Ah
	int 21h
	jmp R5
R4:
	mov bx, 521
	mov al, 115
	mov DS:[bx], al
	add bx, 1
	mov al, 97
	mov DS:[bx], al
	add bx, 1
	mov al, 98
	mov DS:[bx], al
	add bx, 1
	mov al, 101
	mov DS:[bx], al
	add bx, 1
	mov al, 32
	mov DS:[bx], al
	add bx, 1
	mov al, 108
	mov DS:[bx], al
	add bx, 1
	mov al, 101
	mov DS:[bx], al
	add bx, 1
	mov al, 114
	mov DS:[bx], al
	add bx, 1
	mov al, 63
	mov DS:[bx], al
	add bx, 1
	mov al, 24h
	mov DS:[bx], al
	mov dx, 521
	mov ah, 09h
	int 21h
	mov ah, 02h
	mov dl, 0Dh
	int 21h
	mov dl, 0Ah
	int 21h
R5:
	mov ah, 4Ch
	int 21h
cseg ENDS
END strt