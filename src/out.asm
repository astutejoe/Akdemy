section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	x:	resb 65536
section	.text
global	_start
_start:
	mov ebx, temporaries+0
	mov al, 72
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 108
	mov [ebx], al
	add ebx, 1
	mov al, 108
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 87
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 108
	mov [ebx], al
	add ebx, 1
	mov al, 100
	mov [ebx], al
	add ebx, 1
	mov al, 33
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov eax, temporaries+0
	mov ebx, x
Lbl1:
	mov cl, [eax]
	cmp cl, 0x00
	je Lbl2
	mov [ebx], cl
	add eax, 1
	add ebx, 1
	jmp Lbl1
Lbl2:
	mov [ebx], cl
	mov eax, temporaries+0
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
	mov ebx, temporaries+0
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
	mov ecx, temporaries+0
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov ebx, temporaries+0
	mov al, 87
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 105
	mov [ebx], al
	add ebx, 1
	mov al, 116
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 121
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 117
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 110
	mov [ebx], al
	add ebx, 1
	mov al, 97
	mov [ebx], al
	add ebx, 1
	mov al, 109
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 58
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+0
Lbl7:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl8
	add ebx, 1
	add edx, 1
	jmp Lbl7
Lbl8:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+0
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 3
	mov ebx, 0
	mov ecx, temporaries+0
	mov edx, 65536
	int 0x80
	mov eax, x
	mov ebx, temporaries+0
Lbl10:
	mov cl, [ebx]
	cmp cl, 0x0A
	je Lbl9
	mov [eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl10
Lbl9:
	mov cl, 0x00
	mov [eax], cl
	mov ebx, temporaries+65536
	mov al, 89
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 117
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 110
	mov [ebx], al
	add ebx, 1
	mov al, 97
	mov [ebx], al
	add ebx, 1
	mov al, 109
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 105
	mov [ebx], al
	add ebx, 1
	mov al, 115
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+65536
Lbl11:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl12
	add ebx, 1
	add edx, 1
	jmp Lbl11
Lbl12:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65536
	int 0x80
	mov eax, temporaries+131072
	mov ebx, x
Lbl14:
	mov cl, [ebx]
	cmp cl, 0x00
	je Lbl13
	mov [eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl14
Lbl13:
	mov [eax], cl
	mov edx, 0
	mov ebx, temporaries+131072
Lbl15:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl16
	add ebx, 1
	add edx, 1
	jmp Lbl15
Lbl16:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+131072
	int 0x80
	mov ebx, temporaries+196608
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 58
	mov [ebx], al
	add ebx, 1
	mov al, 51
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+196608
Lbl17:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl18
	add ebx, 1
	add edx, 1
	jmp Lbl17
Lbl18:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+196608
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 0x01
	int 0x80