section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	x:	resb 1
section	.text
global	_start
_start:
	mov eax, 3
	mov ebx, 0
	mov ecx, temporaries+0
	mov edx, 65536
	int 0x80
	mov edi, temporaries+0
	mov al, 0
	mov cl, 10
Lbl1:
	mov bl, [edi]
	cmp bl, 0x0A
	je Lbl2
	mul cl
	add bl, -48
	add al, bl
	add edi, 1
	jmp Lbl1
Lbl2:
	mov [x], al
	mov al, [x]
	mov [temporaries+65536], al
	mov edi, temporaries+65537
	mov eax, 0
	mov al, [temporaries+65536]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl3
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl3:
	mov ebx, 10
Lbl4:
	add cx, 1
	add esi, 1
	mov edx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl4
Lbl5:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl5
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65537
	mov edx, esi
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 0x01
	int 0x80