import React, {useState, useEffect} from 'react';

export default function Stake() {

    const [stake, setStake] = useState("");

    useEffect(() => {
        getWalletAmount();
    });

    const getWalletAmount = () => {

        fetch("http://127.0.0.1:8001/nodewallet")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                setStake(jsonData.nodestake);
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    return (
        <pre>
            <small>Stake: {stake} Staked Prawn</small>
            <br/>
        </pre>
    );
}