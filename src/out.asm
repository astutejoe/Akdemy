section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	x:	resw 1
section	.text
global	_start
_start:
	mov eax, -1000000
	mov [x], eax
	mov eax, [x]
	mov [temporaries+0], eax
	mov edi, temporaries+4
	mov eax, 0
	mov eax, [temporaries+0]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge R1
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
R1:
	mov ebx, 10
R2:
	add cx, 1
	add esi, 1
	mov dx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne R2
R3:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne R3
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