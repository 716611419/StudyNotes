	area first, code, readonly
	code32
	entry
start
	 ; һ������֧�����ʱ���ü���
	 ; if ��r0 == r1) 
	 ;    mov r2, #2
	 mov r0, #1
	 mov r1, #2
	 ;subs r0, r0, r1	;������ ��֧�����У� ���ǳ�����Ҫ�ж�����ֵ��С �Ƿ���ȣ� ���ֲ��뱣�������
	 ; ������������� �Ƚ�ָ��
	 ; cmp ָ�� ����Ч����ͬ�� SUBS������������
	 cmp r0, r1
	 moveq r2, #2

	 ; if ��r0 > r1) 
	 ;    mov r2, #2
   	 mov r0, #1
	 mov r1, #2
	 subs r0, r0, r1
	 movgt r2, #2

  	 ; if ��r0 < r1) 
	 ;    mov r2, #2
   	 mov r0, #1
	 mov r1, #2
	 subs r0, r0, r1
	 movlt r2, #2

stop 
	b stop 
	end