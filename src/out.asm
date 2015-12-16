section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	x:	resb 65536
section	.text
global	_start
_start:
	mov eax, 3
	mov ebx, 0
	mov ecx, temporaries+0
	mov edx, 65536
	int 0x80
	mov eax, x
	mov ebx, temporaries+0
Lbl2:
	mov cl, [ebx]
	cmp cl, 0x0A
	je Lbl1
	mov [eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl2
Lbl1:
	mov cl, 0x00
	mov [eax], cl
	mov eax, temporaries+65536
	mov ebx, x
Lbl4:
	mov cl, [ebx]
	cmp cl, 0x00
	je Lbl3
	mov [eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl4
Lbl3:
	mov [eax], cl
	mov edx, 0
	mov ebx, temporaries+65536
Lbl5:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl6
	add ebx, 1
	add edx, 1
	jmp Lbl5
Lbl6:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65536
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 0x01
	int 0x80