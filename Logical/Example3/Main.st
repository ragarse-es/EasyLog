(*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
*********************************************************************************
 * Program: Example3 
 * File: Example 3 - EasyLogLibrary 
 * Author:    B&R Spain
 * Created:   November 18, 2024/10:04 AM 
*********************************************************************************
 * Description: Example to show how to insert changes in a recipe in the logger
	-
 * Interface:
	-
*************************** VERSION CONTROL *************************************
   Version	| Date	                | Author	| Description
----------------------------------------------------------------------------------
   V1.00	| November 18, 2024		| B&R Spain	|  Initial version	
 *********************************************************************************)


PROGRAM _INIT
	(* Set an initial message *)
	Message := 'TestMessage';
	
	//Set the initial Values of the Recipe
	Recipe.MaxTemp := 50;
	Recipe.Name := 'Recipe1';
	
	//Copy the initial value to the Old Recipe
	brsmemcpy(ADR(RecipeOld), ADR(Recipe), SIZEOF(Recipe));
		
	//Repeat until finish the process, with or without errors
	// This will create the logger if it does not exist
	REPEAT
		Status := EasyLog(FALSE, 'RcpChanges', arEVENTLOG_SEVERITY_INFO, 123,  Message);
		UNTIL Status <> ERR_FUB_BUSY 
	END_REPEAT;
		
END_PROGRAM

PROGRAM _CYCLIC
	

	(**********************************  Parameter Name  **********************************)
	// Compare if the recipe has changed
	IF 	brsmemcmp(ADR(RecipeOld.Name), ADR(Recipe.Name), SIZEOF(Recipe.Name)) <> 0 THEN
		//Creation of the message
		Message := CONCAT('Parameter Name of the recipe has changed. Old value = ', RecipeOld.Name);		
		Message := CONCAT(Message, '. New value = ');
		Message := CONCAT(Message, Recipe.Name);
		
		//Register the change
		Status := EasyLog(FALSE, 'RcpChanges', arEVENTLOG_SEVERITY_INFO, 123,  Message);
	
		IF Status <> ERR_OK AND Status <> ERR_FUB_BUSY THEN
			//Check the error and solve it
		END_IF;
		
		//Save the new value
		RecipeOld.Name := Recipe.Name;
	END_IF;


	
	(**********************************  Parameter MaxTemp  **********************************)
	// Compare if the recipe has changed
	IF 	brsmemcmp(ADR(RecipeOld.MaxTemp), ADR(Recipe.MaxTemp), SIZEOF(Recipe.MaxTemp)) <> 0 THEN
		//Creation of the message
		brsitoa(RecipeOld.MaxTemp, ADR(AuxStr));
		Message := CONCAT('Parameter MaxTemp of the recipe has changed. Old value = ', AuxStr);		
		Message := CONCAT(Message, '. New value = ');
		brsitoa(Recipe.MaxTemp, ADR(AuxStr));
		Message := CONCAT(Message, AuxStr);	
		
		//Register the change
		Status := EasyLog(FALSE, 'RcpChanges', arEVENTLOG_SEVERITY_INFO, 123,  Message);	
	
		IF Status <> ERR_OK AND Status <> ERR_FUB_BUSY THEN
			//Check the error and solve it
		END_IF;
		
		//Save the new value
		RecipeOld.MaxTemp := Recipe.MaxTemp;
	END_IF;	
	
	
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

