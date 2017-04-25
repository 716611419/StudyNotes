	area first, code, readonly
	code32
	entry 
start
    ; ָ���ʽ mov rd, op2
	; 1. �ڶ��������������������� ����������ֱ�Ӵ洢��ָ���е����֣�
	;    ��Ϊ ARM ��ָ�����ֻ�� 32 bit, ��ˣ� ��������ָ�����������ʹ�õĹ淶�� ʹ�ù淶�ο�ARMָ�����
	mov r0, #1 	;#������������  
	mov r0, #2 	;#������������  
	mov r0, #3 	;#������������  
	mov r0, #4 	;#������������  
	; �鿴����ָ��Ļ����룬�۲��������Ĺ���
   
    ; ���������� 0-255֮�������ѭ������ż��λ���� 	  
    mov r0, #0xff000000  ; 0x000000ff ѭ������ 8 bit =�� ��Ӧ�������� 0xE3A004FF
	; �Ϸ����������ж�  #200  0x13800000, 0x13810000
	                          ;0b 0001,0011,1000,0000,0000,0000,0000,0000 
	;0xffffffff �ǺϷ���������
	mov r0, #0xffffffff ; MVN ָ�������
	mvn r0, #0x1 ; MVN ָ��ڶ���������λȡ���ٴ���

	; 2. �ڶ��������������ǼĴ���
	mov r0, r1

	; �ڶ������� �Ĵ�����λ������ 5����λ��ʽ�� 9���﷨
	;�߼�����
	mov r0, #0x1
	mov r1, r0, lsl #1    ; ��λλ��1-31�϶��Ϸ�
	
	mov r0, #0x2
	mov r1, r0, lsr #1	   ; �߼�����

	mov r0, #0xffffffff
	mov r1, r0, asr #1	   ; �������Ʒ���λ���䣬 �θ�λ������λ

	mov r0, #0x7fffffff
	mov r1, r0, asr #1

   	mov r0, #0x7fffffff
	mov r1, r0, ror #1	; ѭ������

   	mov r0, #0xffffffff
	mov r1, r0, rrx	; Ψһ����Ҫָ��ѭ��λ������λ��ʽ

	; ��λֵ��������һ���Ĵ�����ֵ��8bit�� д������ 
	mov r2, #1
	mov r0, #0x1
	mov r1, r0, lsl r2    ; ��λλ��1-31�϶��Ϸ�
c
	mov r0, #0xffffffff
	mov r1, r0, asr r2	   ; �������Ʒ���λ���䣬 �θ�λ������λ

	mov r0, #0x7fffffff
	mov r1, r0, asr r2

   	mov r0, #0x7fffffff
	mov r1, r0, ror r2	; ѭ������

	; �� S��׺��Ӱ�� CPSR ���£� CMP CMN TST TEQ ����
	mov r0, #1
	mov r1, r0, lsr #1
	movs r1, r0, lsr #1

stop 
	b stop  

		 
	end