section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	x:	resb 65536
	y:	resb 65536
section	.text
global	_start
_start:
	mov ebx, x
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
	mov ebx, x
Lbl2:
	mov cl, [ebx]
	cmp cl, 0x00
	je Lbl1
	mov [temporaries+eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl2
Lbl1:
	mov [temporaries+eax], cl
	mov eax, 65536
	mov ebx, y
Lbl4:
	mov cl, [ebx]
	cmp cl, 0x00
	je Lbl3
	mov [temporaries+eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl4
Lbl3:
	mov [temporaries+eax], cl
	mov edi, temporaries+0
	mov ebx, temporaries+65536
Lbl9:
	mov al, [edi]
	cmp al, 0x00
	je Lbl10
	mov cl, [ebx]
	cmp cl, 0x00
	je Lbl8
	cmp al, cl
	jne Lbl8
	add ebx, 1
	add edi, 1
	jmp Lbl9
Lbl7:
	mov al, 255
	mov [temporaries+65536], al
	jmp Lbl11
Lbl10:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl7
Lbl8:
	mov al, 0
	mov [temporaries+65536], al
Lbl11:
	mov al, [temporaries+65536]
	cmp al, 0
	jne Lbl12
	mov edi, temporaries+131072
	mov al, 70
	mov [edi], al
	add edi, 1
	mov al, 65
	mov [edi], al
	add edi, 1
	mov al, 76
	mov [edi], al
	add edi, 1
	mov al, 83
	mov [edi], al
	add edi, 1
	mov al, 69
	mov [edi], al
	add edi, 1
	mov al, 36
	mov [edi], al
	mov edx, 5
	jmp Lbl13
Lbl12:
	mov edi, temporaries+131072
	mov al, 84
	mov [edi], al
	add edi, 1
	mov al, 82
	mov [edi], al
	add edi, 1
	mov al, 85
	mov [edi], al
	add edi, 1
	mov al, 69
	mov [edi], al
	add edi, 1
	mov al, 36
	mov [edi], al
	mov edx, 4
Lbl13:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+131072
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 0x01
	int 0x80