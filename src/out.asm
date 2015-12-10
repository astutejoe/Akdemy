section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	y:	resb 65536
section	.text
global	_start
_start:
	mov ebx, y
	mov al, 79
	mov [ebx], al
	add ebx, 1
	mov al, 108
	mov [ebx], al
	add ebx, 1
	mov al, 97
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 77
	mov [ebx], al
	add ebx, 1
	mov al, 117
	mov [ebx], al
	add ebx, 1
	mov al, 110
	mov [ebx], al
	add ebx, 1
	mov al, 100
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 0
	mov [ebx], al
	mov eax, 0
	mov ebx, 0
R2:
	mov cl, [y+ebx]
	cmp cl, 0x00
	je R1
	mov [temporaries+eax], cl
	add eax, 1
	add ebx, 1
	jmp R2
R1:
	mov [temporaries+eax], cl
	mov edx, 0
	mov ebx, temporaries+0
R3:
	mov al, [ebx]
	cmp al, 0x00
	je R4
	add ebx, 1
	add edx, 1
	jmp R3
	R4:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+0
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 0x01
	int 0x80