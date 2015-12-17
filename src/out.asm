section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	t1:	resd 1
	t2:	resd 1
	tmp:	resd 1
	n:	resd 1
	i:	resd 1
section	.text
global	_start
_start:
	mov eax, +1
	mov [t1], eax
	mov eax, +1
	mov [t2], eax
	mov eax, +2
	mov [i], eax
	mov ebx, temporaries+0
	mov al, 69
	mov [ebx], al
	add ebx, 1
	mov al, 115
	mov [ebx], al
	add ebx, 1
	mov al, 99
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 118
	mov [ebx], al
	add ebx, 1
	mov al, 97
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 110
	mov [ebx], al
	add ebx, 1
	mov al, 117
	mov [ebx], al
	add ebx, 1
	mov al, 109
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 100
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 116
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 109
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 115
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 40
	mov [ebx], al
	add ebx, 1
	mov al, 109
	mov [ebx], al
	add ebx, 1
	mov al, 97
	mov [ebx], al
	add ebx, 1
	mov al, 105
	mov [ebx], al
	add ebx, 1
	mov al, 111
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 113
	mov [ebx], al
	add ebx, 1
	mov al, 117
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 50
	mov [ebx], al
	add ebx, 1
	mov al, 41
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
Lbl1:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl2
	add ebx, 1
	add edx, 1
	jmp Lbl1
Lbl2:
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
	mov edi, temporaries+0
	mov eax, 0
	mov ecx, 10
	mov esi, 1
	mov bl, [edi]
	cmp bl, 2Dh
	jne Lbl3
	mov esi, -1
	add edi, 1
	mov bl, [edi]
Lbl3:
	mov ebx, 0
	mov bl, [edi]
	cmp bl, 0x0A
	je Lbl4
	imul ecx
	add bl, -48
	add eax, ebx
	add edi, 1
	jmp Lbl3
Lbl4:
	mov edx, esi
	imul edx
	mov [n], eax
	mov eax, [n]
	mov [temporaries+65536], eax
	mov al, 2
	mov [temporaries+65540], al
	mov eax, [temporaries+65536]
	mov ebx, 0
	mov bl, [temporaries+65540]
	cmp eax, ebx
	jg Lbl8
	mov al, 0
	jmp Lbl9
Lbl8:
	mov al, 255
Lbl9:
	mov [temporaries+65540], al
	mov al, [temporaries+65540]
	cmp al, 0
	je Lbl6
	mov ebx, temporaries+65541
	mov al, 91
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+65541
Lbl10:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl11
	add ebx, 1
	add edx, 1
	jmp Lbl10
Lbl11:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65541
	int 0x80
	mov eax, [t1]
	mov [temporaries+131077], eax
	mov edi, temporaries+131081
	mov eax, 0
	mov eax, [temporaries+131077]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl12
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl12:
	mov ebx, 10
Lbl13:
	add cx, 1
	add esi, 1
	mov edx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl13
Lbl14:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl14
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+131081
	mov edx, esi
	int 0x80
	mov ebx, temporaries+196617
	mov al, 44
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+196617
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
	mov ecx, temporaries+196617
	int 0x80
	mov eax, [t2]
	mov [temporaries+262153], eax
	mov edi, temporaries+262157
	mov eax, 0
	mov eax, [temporaries+262153]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl17
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl17:
	mov ebx, 10
Lbl18:
	add cx, 1
	add esi, 1
	mov edx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl18
Lbl19:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl19
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+262157
	mov edx, esi
	int 0x80
	mov eax, [n]
	mov [temporaries+65541], eax
	mov al, 2
	mov [temporaries+65545], al
	mov eax, [temporaries+65541]
	mov ebx, 0
	mov bl, [temporaries+65545]
	cmp eax, ebx
	jg Lbl22
	mov al, 0
	jmp Lbl23
Lbl22:
	mov al, 255
Lbl23:
	mov [temporaries+65545], al
	mov al, [temporaries+65545]
	cmp al, 0
	je Lbl20
	mov ebx, temporaries+65546
	mov al, 44
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+65546
Lbl24:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl25
	add ebx, 1
	add edx, 1
	jmp Lbl24
Lbl25:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65546
	int 0x80
	jmp Lbl21
Lbl20:
Lbl21:
Lbl26:
	mov eax, [i]
	mov [temporaries+65546], eax
	mov eax, [n]
	mov [temporaries+65550], eax
	mov eax, [temporaries+65546]
	mov ebx, [temporaries+65550]
	cmp eax, ebx
	jl Lbl28
	mov al, 0
	jmp Lbl29
Lbl28:
	mov al, 255
Lbl29:
	mov [temporaries+65550], al
	mov al, [temporaries+65550]
	cmp al, 0
	je Lbl27
	mov eax, [t2]
	mov [temporaries+65554], eax
	mov eax, [temporaries+65554]
	mov [tmp], eax
	mov eax, [t1]
	mov [temporaries+65554], eax
	mov eax, [t2]
	mov [temporaries+65558], eax
	mov eax, [temporaries+65554]
	mov ebx, [temporaries+65558]
	add eax, ebx
	mov [temporaries+65558], eax
	mov eax, [temporaries+65558]
	mov [t2], eax
	mov eax, [t2]
	mov [temporaries+65554], eax
	mov edi, temporaries+65558
	mov eax, 0
	mov eax, [temporaries+65554]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl30
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl30:
	mov ebx, 10
Lbl31:
	add cx, 1
	add esi, 1
	mov edx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl31
Lbl32:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl32
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65558
	mov edx, esi
	int 0x80
	mov eax, [i]
	mov [temporaries+65554], eax
	mov eax, [n]
	mov [temporaries+65558], eax
	mov al, 1
	mov [temporaries+65562], al
	mov eax, [temporaries+65558]
	mov ebx, 0
	mov bl, [temporaries+65562]
	sub eax, ebx
	mov [temporaries+65558], eax
	mov eax, [temporaries+65554]
	mov ebx, [temporaries+65558]
	cmp eax, ebx
	jl Lbl35
	mov al, 0
	jmp Lbl36
Lbl35:
	mov al, 255
Lbl36:
	mov [temporaries+65558], al
	mov al, [temporaries+65558]
	cmp al, 0
	je Lbl33
	mov ebx, temporaries+65563
	mov al, 44
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+65563
Lbl37:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl38
	add ebx, 1
	add edx, 1
	jmp Lbl37
Lbl38:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65563
	int 0x80
	jmp Lbl34
Lbl33:
Lbl34:
	mov eax, [i]
	mov [temporaries+65563], eax
	mov al, 1
	mov [temporaries+65567], al
	mov eax, [temporaries+65563]
	mov ebx, 0
	mov bl, [temporaries+65567]
	add eax, ebx
	mov [temporaries+65563], eax
	mov eax, [temporaries+65563]
	mov [i], eax
	mov eax, [tmp]
	mov [temporaries+65563], eax
	mov eax, [temporaries+65563]
	mov [t1], eax
	jmp Lbl26
Lbl27:
	mov ebx, temporaries+65546
	mov al, 93
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+65546
Lbl39:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl40
	add ebx, 1
	add edx, 1
	jmp Lbl39
Lbl40:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65546
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	jmp Lbl7
Lbl6:
	mov ebx, temporaries+65546
	mov al, 115
	mov [ebx], al
	add ebx, 1
	mov al, 97
	mov [ebx], al
	add ebx, 1
	mov al, 98
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 32
	mov [ebx], al
	add ebx, 1
	mov al, 108
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 114
	mov [ebx], al
	add ebx, 1
	mov al, 63
	mov [ebx], al
	add ebx, 1
	mov al, 0x00
	mov [ebx], al
	mov edx, 0
	mov ebx, temporaries+65546
Lbl41:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl42
	add ebx, 1
	add edx, 1
	jmp Lbl41
Lbl42:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65546
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
Lbl7:
	mov eax, 0x01
	int 0x80