section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	i:	resd 1
	a:	resd 1
section	.text
global	_start
_start:
	mov al, 5
	mov [temporaries+0], al
	mov al, [temporaries+0]
	mov ah, 0
	mov [temporaries+16384], al
	mov [temporaries+16385], ah
	mov eax, [a]
	mov [temporaries+0], eax
	mov al, [temporaries+0]
	mov ah, [temporaries+1]
	mov [temporaries+16384], al
	mov [temporaries+16385], ah
Lbl1:
	mov eax, [i]
	mov [temporaries+0], eax
	mov al, 1
	mov [temporaries+4], al
	mov al, [temporaries+0]
	mov ah, [temporaries+1]
	mov bl, [temporaries+4]
	mov bh, 0
	cmp ax, bx
	jg Lbl3
	mov al, 0
	jmp Lbl4
Lbl3:
	mov al, 255
Lbl4:
	mov [temporaries+4], al
	mov al, [temporaries+4]
	mov ah, 0
	cmp ax, 0
	je Lbl2
	mov eax, [a]
	mov [temporaries+5], eax
	mov eax, [i]
	mov [temporaries+9], eax
	mov al, 1
	mov [temporaries+13], al
	mov al, [temporaries+9]
	mov ah, [temporaries+10]
	mov bl, [temporaries+13]
	mov bh, 0
	sub ax, bx
	mov [temporaries+13], al
	mov [temporaries+14], ah
	mov al, [temporaries+5]
	mov ah, [temporaries+6]
	mov bl, [temporaries+13]
	mov bh, [temporaries+14]
	imul bx
	mov [temporaries+13], al
	mov [temporaries+14], ah
	mov al, [temporaries+13]
	mov ah, [temporaries+14]
	mov [temporaries+16384], al
	mov [temporaries+16385], ah
	mov eax, [i]
	mov [temporaries+5], eax
	mov al, 1
	mov [temporaries+9], al
	mov al, [temporaries+5]
	mov ah, [temporaries+6]
	mov bl, [temporaries+9]
	mov bh, 0
	sub ax, bx
	mov [temporaries+9], al
	mov [temporaries+10], ah
	mov al, [temporaries+9]
	mov ah, [temporaries+10]
	mov [temporaries+16384], al
	mov [temporaries+16385], ah
	jmp Lbl1
Lbl2:
	mov eax, [a]
	mov [temporaries+0], eax
	mov edi, temporaries+4
	mov eax, 0
	mov eax, [temporaries+0]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl5
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl5:
	mov ebx, 10
Lbl6:
	add cx, 1
	add esi, 1
	mov dx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl6
Lbl7:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl7
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+4
	mov edx, esi
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 0x01
	int 0x80