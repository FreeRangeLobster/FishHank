   M S B A M L   `   `   `  ����  
SerialComm,    &SerialComm.View.SerialPortSettingsView   5      R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5      @d2http://schemas.microsoft.com/expression/blend/2008     5      .��5      �   � RowDefinitions  5	   
   �� 5
      �� 5      .+�5   
   �� $��0��6   $��Serial Port Settings��6       GroupBoxStyleOverride#��[  6>   .��5      �   5      �� 5      �� 5      �� 5      �� 5      �� 5      .+�5      �� $��0��6    �� Orientation$ 
Horizontal=�6*   $��0 5 0 5q�6C   .+�5      �� $��
COM* port:��6   $��90��61   ���	#FF5555556<   �� 5       �� DisplayMemberPath$ 
DeviceInfo��6A   $	��100��6�   $	��300��6�   ��6   �� *	CommPorts+ �� SelectedItem 6`   �� *SelectedCommPort+ �� Mode$ TwoWay=�{�5   '   �� *EnableDisableSettings+�� 5      $��5 0 0 0q�6    � ToolTip$ Refresh COM* ports��6.   ��6K   �� *RefreshPorts+.��5      F� �����PQ��` �@1� ��J ���  	=  �  	= � 	=  �   z  z � �  -1  z  z �P�  -1�yD
P-��� �PQ��� S0� �G�zT0@P�� �� � ��׀�[  � ���[ ��[ � � ��[ ���[ ��[ �`o� ��[ �� �f ���Q80@��v �@]� �ا � -1�ا � -1 	= �PQ��` 	  ForegroundBrush#��[ 6�    �� Stretch$ Uniform=�6!  5      5      �� 5      $��1��6   $ 
Horizontal=�6*   $��0 0 0 5q�6C   .+�5      �� $��
Baud rate:��6   $��90��61   ���	#FF5555556<   �� 5       $ BaudRateName��6A    �� SelectedValuePath$ BaudRateValue��6�   $	��100��5!   S   ��5       �� *	BaudRates+ �� SelectedValue 6b   �� *SelectedBaudRate+$ TwoWay=�{�5!   '   �� *EnableDisableSettings+6S   �� 5"      $��Parity:��6   $��20 0 0 0q�6.   ���	#FF5555556@   �� 5#      $ 
ParityName��6@   $ ParityValue��6�   $	��100��5$   S   ��5#      �� *
Parities+ 6_   �� *SelectedParity+$ TwoWay=�{�5$   '   �� *EnableDisableSettings+6S   5%      �� 5&      $��2��6   $ 
Horizontal=�6*   $��0 0 0 5q�6C   .+�5'      �� $��
Data bits:��6   $��90��61   ���	#FF5555556<   �� 5(      $��70��5)   S   ��5(      �� *
DataBits+ 6@   �� *SelectedDataBits+$ TwoWay=�{�5)   '   �� *EnableDisableSettings+6S   �� 5*      $��
Stop bits:��6   $��20 0 0 0q�61   ���	#FF5555556C   �� 5+      $ StopBitsName��6D   $ StopBitsValue��6�   $	��100��5,   S   ��5+      �� *StopBitsList+ 6e   �� *SelectedStopBits+$ TwoWay=�{�5,   '   �� *EnableDisableSettings+6S   5-      �� 5.      $��3��6   $ 
Horizontal=�6*   $��0 0 0 5q�6C   .+�5/      �� $��Center=�6   $��Center=�6:   $��Data Terminal Ready��6�   $��85 0 0 0q�6�   	 d� 	IsChecked	 6\   �� *IsDTR+$ TwoWay=�{�50   '   �� *EnableDisableSettings+�� 51      $��Center=�6   $��Center=�6:   $��Request To Send��6�   $��20 0 0 0q�6�   	 6\   �� *IsRTS+$ TwoWay=�{�52   '   �� *EnableDisableSettings+53      54      55      �� 56   
   $��1��6   $��Switch��6    $��0 5 0 0q�60   #��[  6A   .��57      � 
 � ColumnDefinitions
 58      �� 59      �� 5:      .+�5<      �� $��0��6   $��START COMMUNICATION��6)   $
��10 3q�6`   $��5q�6o   
��� � ���6G   �� *Open+5=   !   �� 5>      $��1��6   $��STOP COMMUNICATION��6)   $
��10 3q�6`   $��5q�6o   
���  ����6F   �� *Close+5?   !   5@      5A      5B      5C      