section	.data
	endl:	 db 0x0A
section	.bss
	tmp:	resb 8388608
	y:	resb 65536
section	.text
global	_start
_start:
	mov ebx, y
	mov al, 71
	mov [ebx], al
	add ebx, 1
	mov al, 97
	mov [ebx], al
	add ebx, 1
	mov al, 98
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 105
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 108
	mov [ebx], al
	add ebx, 1
	mov al, 0
	mov [ebx], al
	mov eax, 0
	mov ebx, 0
R2:
	mov ecx, [y+ebx]
	cmp ecx, 0x00
	je R1
	mov [tmp+eax], ecx
	add eax, 1
	add ebx, 1
	jmp R2
R1:
	mov [tmp+eax], ecx
	mov eax, 4
	mov ebx, 1
	mov ecx, tmp+0
	mov edx, 7
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 0x01
	int 0x80