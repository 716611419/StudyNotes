	area first, code, readonly
	code32
	entry
start
	; bl ָ��, �Ӻ�������
	bl child_func  ;0xeb 00 00 01  ע��BL ����ת���ƣ���Ϊָ������ƣ�BL��ת��Χ���ޣ� ��BL�Ļ��������淶
	nop
	;while (1);
stop 
	b stop 


; �Ӻ���
child_func 
	mov r0, #1	; <===  pc
	; bl child_func1
	;�Ӻ�������
	mov pc, lr

	end
