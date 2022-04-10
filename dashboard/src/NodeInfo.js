import React, {useState, useEffect} from 'react';

export default function NodeInfo() {

    const [nodeid, setNodeid] = useState("");
    const [ip, setIp] = useState("");
    const [port, setPort] = useState("");
    const [master, setMaster] = useState("");

    useEffect(() => {
        getNodeID();
    }, []);

    const getNodeID = () => {

        fetch("http://127.0.0.1:8001/node")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                setNodeid(jsonData.message);
                setIp(jsonData.ip);
                setPort(jsonData.port);
                setMaster(jsonData.master);
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }


    return (
        <div>
        <b>Node</b><br/>
        <small>
        ID: {nodeid}<br/>
        Node IP: {ip}<br/>
        Node Port: {port}<br/>
        Master Node: {master}<br/><br/>
        <button className="btn btn-primary">Configure Node</button>
        </small>
        </div>
    );
    
}