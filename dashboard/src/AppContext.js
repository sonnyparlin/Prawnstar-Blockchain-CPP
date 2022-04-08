import React, {useState, createContext} from 'react';
export const AppContext = createContext();
export const AppProvider = (props) => {
    const [selectedTrans, setSelectedTrans] = useState(null);
    return (
        <AppContext.Provider value={{selectedTrans, setSelectedTrans}}> 
            {props.children}
        </AppContext.Provider>
    );
}

export default AppContext;