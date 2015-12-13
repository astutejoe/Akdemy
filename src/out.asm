section	.data
	endl:	 db 0x0A
section	.bss
	temporaries:	resb 8388608
	x:	resd 1
	y:	resd 1
	z:	resd 1
	a:	resb 1
	b:	resb 1
	c:	resb 1
	d:	resb 65536
	e:	resb 65536
	f:	resb 65536
section	.text
global	_start
_start:
	mov eax, +5
	mov [x], eax
	mov eax, +6
	mov [y], eax
	mov eax, +7
	mov [z], eax
	mov al, 1
	mov [a], al
	mov al, 2
	mov [b], al
	mov al, 3
	mov [c], al
	mov ebx, d
	mov al, 84
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 115
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
	mov al, 0
	mov [ebx], al
	mov ebx, e
	mov al, 71
	mov [ebx], al
	add ebx, 1
	mov al, 101
	mov [ebx], al
	add ebx, 1
	mov al, 110
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
	mov al, 0
	mov [ebx], al
	mov ebx, f
	mov al, 70
	mov [ebx], al
	add ebx, 1
	mov al, 105
	mov [ebx], al
	add ebx, 1
	mov al, 110
	mov [ebx], al
	add ebx, 1
	mov al, 97
	mov [ebx], al
	add ebx, 1
	mov al, 0
	mov [ebx], al
	mov eax, [x]
	mov [temporaries+0], eax
	mov edi, temporaries+4
	mov eax, 0
	mov eax, [temporaries+0]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl1
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl1:
	mov ebx, 10
Lbl2:
	add cx, 1
	add esi, 1
	mov dx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl2
Lbl3:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl3
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+4
	mov edx, esi
	int 0x80
	mov eax, [y]
	mov [temporaries+65540], eax
	mov edi, temporaries+65544
	mov eax, 0
	mov eax, [temporaries+65540]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl4
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl4:
	mov ebx, 10
Lbl5:
	add cx, 1
	add esi, 1
	mov dx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl5
Lbl6:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl6
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65544
	mov edx, esi
	int 0x80
	mov eax, [z]
	mov [temporaries+131080], eax
	mov edi, temporaries+131084
	mov eax, 0
	mov eax, [temporaries+131080]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl7
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl7:
	mov ebx, 10
Lbl8:
	add cx, 1
	add esi, 1
	mov dx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl8
Lbl9:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl9
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+131084
	mov edx, esi
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov al, [a]
	mov [temporaries+0], al
	mov edi, temporaries+1
	mov eax, 0
	mov al, [temporaries+0]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl10
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl10:
	mov ebx, 10
Lbl11:
	add cx, 1
	add esi, 1
	mov dx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl11
Lbl12:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl12
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+1
	mov edx, esi
	int 0x80
	mov al, [b]
	mov [temporaries+65537], al
	mov edi, temporaries+65538
	mov eax, 0
	mov al, [temporaries+65537]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl13
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl13:
	mov ebx, 10
Lbl14:
	add cx, 1
	add esi, 1
	mov dx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl14
Lbl15:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl15
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65538
	mov edx, esi
	int 0x80
	mov al, [c]
	mov [temporaries+131074], al
	mov edi, temporaries+131075
	mov eax, 0
	mov al, [temporaries+131074]
	mov cx, 0
	mov esi, 0
	cmp eax, 0
	jge Lbl16
	mov dl, 0x2D
	mov [edi], dl
	add edi, 1
	neg eax
	add esi, 1
Lbl16:
	mov ebx, 10
Lbl17:
	add cx, 1
	add esi, 1
	mov dx, 0
	idiv ebx
	push dx
	cmp eax, 0
	jne Lbl17
Lbl18:
	pop dx
	add dx, 48
	mov [edi], dl
	add edi, 1
	add cx, -1
	cmp cx, 0
	jne Lbl18
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+131075
	mov edx, esi
	int 0x80
	mov eax, 4
	mov ebx, 1
	mov ecx, endl
	mov edx, 1
	int 0x80
	mov eax, 0
	mov ebx, 0
Lbl20:
	mov cl, [d+ebx]
	cmp cl, 0x00
	je Lbl19
	mov [temporaries+eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl20
Lbl19:
	mov [temporaries+eax], cl
	mov edx, 0
	mov ebx, temporaries+0
Lbl21:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl22
	add ebx, 1
	add edx, 1
	jmp Lbl21
Lbl22:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+0
	int 0x80
	mov eax, 65536
	mov ebx, 0
Lbl24:
	mov cl, [e+ebx]
	cmp cl, 0x00
	je Lbl23
	mov [temporaries+eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl24
Lbl23:
	mov [temporaries+eax], cl
	mov edx, 0
	mov ebx, temporaries+65536
Lbl25:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl26
	add ebx, 1
	add edx, 1
	jmp Lbl25
Lbl26:
	mov eax, 4
	mov ebx, 1
	mov ecx, temporaries+65536
	int 0x80
	mov eax, 131072
	mov ebx, 0
Lbl28:
	mov cl, [f+ebx]
	cmp cl, 0x00
	je Lbl27
	mov [temporaries+eax], cl
	add eax, 1
	add ebx, 1
	jmp Lbl28
Lbl27:
	mov [temporaries+eax], cl
	mov edx, 0
	mov ebx, temporaries+131072
Lbl29:
	mov al, [ebx]
	cmp al, 0x00
	je Lbl30
	add ebx, 1
	add edx, 1
	jmp Lbl29
Lbl30:
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