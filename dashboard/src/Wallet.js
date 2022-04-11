import React, {useState, useEffect} from 'react';

export default function Wallet({setWallet}) {

    const [amount, setAmount] = useState("");
    const [address, setAddress] = useState("");

    useEffect((amount, address) => {
        getWalletAmount();
    });

    // console.log("address from Wallet component is " + address);

    const getWalletAmount = () => {

        fetch("http://127.0.0.1:8001/nodewallet")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                // console.log(jsonData.address);
                setAmount(jsonData.amount);
                setAddress(jsonData.address);
                setWallet(amount, address);
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    return (
        
        <pre>
            <small>Balance: {amount} Prawn</small><br/>
            <small>Address: {address}</small>
        </pre>
    
    );
}