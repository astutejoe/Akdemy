sseg SEGMENT STACK
	byte 4000h DUP(?)
sseg ENDS

dseg SEGMENT PUBLIC
	byte 4000h DUP(?)
	sword ?
	sword ?
dseg ENDS

cseg SEGMENT PUBLIC
	ASSUME CS:cseg, DS:dseg

strt:
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
	mov al, 117
	mov DS:[bx], al
	add bx, 1
	mov al, 109
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
	mov DS:[16386], al
	mov DS:[16387], ah
	mov al, DS:[16386]
	mov ah, DS:[16387]
	mov DS:[515], al
	mov DS:[516], ah
	mov al, DS:[515]
	mov ah, DS:[516]
	mov DS:[16384], al
	mov DS:[16385], ah
R4:
	mov al, DS:[16384]
	mov ah, DS:[16385]
	mov DS:[515], al
	mov DS:[516], ah
	mov al, 1
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
	je R5
	mov al, DS:[16386]
	mov ah, DS:[16387]
	mov DS:[518], al
	mov DS:[519], ah
	mov al, DS:[16384]
	mov ah, DS:[16385]
	mov DS:[520], al
	mov DS:[521], ah
	mov al, 1
	mov DS:[522], al
	mov al, DS:[520]
	mov ah, DS:[521]
	mov bl, DS:[522]
	mov bh, 0
	sub ax, bx
	mov DS:[522], al
	mov DS:[523], ah
	mov al, DS:[518]
	mov ah, DS:[519]
	mov bl, DS:[522]
	mov bh, DS:[523]
	imul bx
	mov DS:[522], al
	mov DS:[523], ah
	mov al, DS:[522]
	mov ah, DS:[523]
	mov DS:[16386], al
	mov DS:[16387], ah
	mov al, DS:[16384]
	mov ah, DS:[16385]
	mov DS:[518], al
	mov DS:[519], ah
	mov al, 1
	mov DS:[520], al
	mov al, DS:[518]
	mov ah, DS:[519]
	mov bl, DS:[520]
	mov bh, 0
	sub ax, bx
	mov DS:[520], al
	mov DS:[521], ah
	mov al, DS:[520]
	mov ah, DS:[521]
	mov DS:[16384], al
	mov DS:[16385], ah
	jmp R4
R5:
	mov al, DS:[16386]
	mov ah, DS:[16387]
	mov DS:[515], al
	mov DS:[516], ah
	mov di, 517
	mov al, DS:[515]
	mov ah, DS:[516]
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
	mov dx, 517
	mov ah, 09h
	int 21h
	mov ah, 02h
	mov dl, 0Dh
	int 21h
	mov dl, 0Ah
	int 21h
	mov ah, 4Ch
	int 21h
cseg ENDS
END strt